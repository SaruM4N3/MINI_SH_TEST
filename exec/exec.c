/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebroue <leobroue@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/27 17:13:00 by lebroue           #+#    #+#             */
/*   Updated: 2025/11/05 19:10:11 by lebroue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exec.h"
#include "parsing.h"

// lose(), read(), write(), dup2(), pipe()

///////////////////////////////
///////GET PATH - UTILS////////

void	ft_free_tab_str(char **str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		free(str[i]);
		i++;
	}
	free(str);
}

//////////////////////////////////////////////
// Générer un nom unique pour chaque heredoc
//////////////////////////////////////////////
char	*get_unique_heredoc_path(void)
{
	int		i;
	char	*path;
	int		fd;
	char	*index;

	i = 0;
	while (1)
	{
		index = ft_itoa(i);
		// verifier que itoa ne casse pas
		path = ft_strjoin("/tmp/heredoc", index);
		free(index);
		fd = open(path, O_CREAT | O_EXCL | O_WRONLY, 0644);
		if (fd != -1)
		{
			close(fd);
			return (path);
		}
		free(path);
		i++;
	}
}

/*
parsing :
- trouver hd
- open 2 fd ( 1 ecriture (parsing) | 1 lecture (exec))
- unlink immediatement le fichier (plus reference sur le filesystem)
- rempli le fd d'ecriture avec le contenu du hd puis close le fd d'ecriture
- envoie le fd de lecture a l'exec,
	qui l'utilisera puis closera une fois les operations effectuees
*/

//////////////////////////////////////////////
// Libérer argv
//////////////////////////////////////////////
void	free_argv(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return ;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
	return ;
}

// //////////////////////////////////////////////
// // Libérer envp
// //////////////////////////////////////////////
void	free_envp(char **envp)
{
	int	i;

	i = 0;
	if (!envp)
		return ;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
}

int	is_single_cmd(t_data *data)
{
	if (!data || !data->cmds)
		return (0);
	return (data->cmds->next == NULL);
}
int	is_builtins(char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "echo") || !ft_strcmp(cmd, "cd") || !ft_strcmp(cmd,
			"pwd") || !ft_strcmp(cmd, "export") || !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env") || !ft_strcmp(cmd, "exit"));
}

void	save_std_in_out(t_data *data)
{
	data->saved_stdin = dup(STDIN_FILENO);
	data->saved_stdout = dup(STDOUT_FILENO);
	if (data->saved_stdin == -1 || data->saved_stdout == -1)
	{
		perror("dup");
		exit(1);
	}
}

void	reset_std_in_out(t_data *data)
{
	if (dup2(data->saved_stdin, STDIN_FILENO) == -1)
	{
		perror("dup2 stdin");
		exit(1);
	}
	if (dup2(data->saved_stdout, STDOUT_FILENO) == -1)
	{
		perror("dup2 stdout");
		exit(1);
	}
	close(data->saved_stdin);
	close(data->saved_stdout);
}

int	waiting(pid_t pid, int status)
{
	int		ret;
	pid_t	pid_at_exit;

	while (1)
	{
		pid_at_exit = wait(&status);
		if (pid_at_exit == pid)
			ret = WEXITSTATUS(status);
		if (pid_at_exit < 0)
			break ;
	}
	return (ret);
}

//////////////////////////////////////////////
// Exécuter la liste de commandes avec pipes
//////////////////////////////////////////////
int	exec_cmd(t_data *data, char *input)
{
	int		ret;
	t_cmd	*curr;
	int		prev_fd;
	int		status;

	pid_t pid;         // PID du processus enfant
	ret = 0;           // Code de retour de la commande
	curr = data->cmds; // Commande courante
	int pipe_fd[2];    // Tableau pour les pipes [0]=lecture, [1]=écriture
	prev_fd = -1;
	status = 0; // FD du pipe précédent pour la redirection stdin
	// Statut utilisé pour récupérer le code de sortie des enfants
	// Met à jour le tableau envp à partir de t_data
	update_envp(data);
	// Si c'est une seule commande et que c'est un builtin,
	// on l'exécute directement dans le processus parent
	if (is_single_cmd(data) /*&& !ft_strncmp(curr->name, "exit", 5)*/)
	{
		if (!ft_strncmp(curr->name, "exit", 5))
		{
			if (apply_redirections(curr) == -1)
			{
				free_all(data, 1, NULL); // Libère la mémoire et met ret=1
				exit(1);                 // Sort de l'enfant
			}
			ret = exec_builtins(curr, data, data->envp, input);
			return (ret);
		}
		if (!ft_strncmp(curr->name, "cd", 2))
		{
			ret = exec_builtins(curr, data, data->envp, input);
			return (ret);
		}
		if (!ft_strncmp(curr->name, "export", 6))
		{
			ret = exec_builtins(curr, data, data->envp, input);
			return (ret);
		}
		if (!ft_strncmp(curr->name, "unset", 5))
		{
			ret = exec_builtins(curr, data, data->envp, input);
			return (ret);
		}
	}
	// Boucle sur toutes les commandes de la pipeline
	while (curr)
	{
		// Création du pipe si ce n'est pas la dernière commande
		if (curr->next && pipe(pipe_fd) == -1)
		{
			perror("pipe");
			return (1);
		}
		// Fork pour créer un enfant qui exécutera la commande
		pid = fork();
		if (pid < 0) // Erreur de fork
		{
			perror("fork");
			return (1);
		}
		else if (pid == 0) // Bloc enfant
		{
			// Si on a un pipe précédent, rediriger stdin
			if (prev_fd != -1)
			{
				dup2(prev_fd, STDIN_FILENO);
				close(prev_fd); // Fermeture du fd après duplication
			}
			// Si ce n'est pas la dernière commande,
			// rediriger stdout vers le pipe
			if (curr->next)
			{
				close(pipe_fd[0]); // On ferme le côté lecture
				dup2(pipe_fd[1], STDOUT_FILENO);
				// Redirige stdout vers l'écriture du pipe
				close(pipe_fd[1]); // Fermeture après duplication
			}
			// Appliquer les redirections (> < >> ...)
			if (apply_redirections(curr) == -1)
			{
				free_all(data, 1, NULL); // Libère la mémoire et met ret=1
				exit(1);                 // Sort de l'enfant
			}
			// Si la commande est un builtin dans un pipeline
			if (is_builtins(curr->name))
			{
				// printf("bonjour\n");
				ret = exec_builtins(curr, data, data->envp, input);
				reset_std_in_out(data);
				free_all(data, ret, NULL); // Libération mémoire
											// Retourne le code de sortie du builtin
			}
			// Commande externe
			curr->path = get_path(data->envp, curr->args[0], &ret);
			if (!curr->path)
			{
				fprintf(stderr, "Command not found: %s\n", curr->args[0]);
				free_all(data, 127, NULL);
				exit(127); // Code 127 pour commande non trouvée
			}
			// Exécuter la commande externe
			execve(curr->path, curr->args, data->envp);
			perror("execve"); // Si execve échoue
			free_all(data, 1, NULL);
			exit(1);
		}
		// Bloc parent : fermer les fd inutiles pour éviter les fuites
		if (prev_fd != -1)
			close(prev_fd);
		if (curr->next)
		{
			close(pipe_fd[1]); // Fermeture côté écriture
			prev_fd = pipe_fd[0];
			// On garde le côté lecture pour la prochaine commande
		}
		// Passer à la commande suivante dans la pipeline
		curr = curr->next;
	}
	// Parent attend tous les enfants pour récupérer leurs codes de sortie
	ret = waiting(pid, status);
	// Récupérer le code de sortie du dernier processus
	return (ret);
}
