#include "minishell.h"

int	main(int ac, char **av, char **env)
{
	char	*line;
	t_env	*envd;
	t_data	data;
	int		running;

	(void)ac;
	(void)av;
	envd = NULL;
	running = 1;
	init_lst_env(&envd, env); // segfault
	save_std_in_out(&data);
	handle_signals();
	while (1) //(running) // flag de running pour continuer la boubcle, si
	{
		line = reader();
		if (!line)
		{
			// running = 0;
			continue ;
		}
		if (line[0] != '\0')
			add_history(line);
		// if(data.envp) // libere lancien envp //
		// a verfier
		init_data(&data, &envd, NULL);
		data.cmds = parsing(line);
		// print_cmds(data.cmds);
		init_envp(&data);
		// print_lst_env(envd);
		g_exit_status = exec_cmd(&data, line);
		if (g_exit_status == -1)
		{
			// printf("feuuuuuuuuuuuuuuuur\n");
			// free_all(&data, 0, "");
			free(line);
			rl_clear_history();
			free_envp(data.envp); // free **envp si exec cmd echoue
			free_cmds(data.cmds); // free
			free_lst_env(&data.env, true, 0);
			// free la liste chaine de l'environement
			return (1);
		}
		free_cmds(data.cmds);
		free(line);
	}
	// on arrive jamais ici
	rl_clear_history();
	free_envp(data.envp); // si exec cmd c'est bien passer, free **envp,
	free_lst_env(&data.env, true, 0);
	return (0);
}

void	print_cmds(t_cmd *cmds)
{
	t_cmd	*tmp;
	int		i;
	t_redir	*r_tmp;

	tmp = cmds;
	while (tmp)
	{
		printf("Command: %s\n", tmp->name);
		if (tmp->args)
		{
			i = 0;
			printf("Arguments:\n");
			while (tmp->args[i])
			{
				printf("  arg[%d]: %s\n", i, tmp->args[i]);
				i++;
			}
		}
		else
		{
			printf("No arguments\n");
		}
		if (tmp->redirs)
		{
			r_tmp = tmp->redirs;
			printf("Redirections:\n");
			while (r_tmp)
			{
				printf("  Type: %d, File: %s\n", r_tmp->type, r_tmp->file);
				r_tmp = r_tmp->next;
			}
		}
		else
		{
			printf("No redirections\n");
		}
		tmp = tmp->next;
	}
}

void	print_lst_env(t_env *envd)
{
	t_env *current;

	current = envd;
	while (current)
	{
		printf("%s=%s\n", current->key, current->value);
		current = current->next;
	}
}