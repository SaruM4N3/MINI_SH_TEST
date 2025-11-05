/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebroue <leobroue@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 18:32:47 by lebroue           #+#    #+#             */
/*   Updated: 2025/11/05 03:56:06 by lebroue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	ft_free_tab(char **dst)
{
	int	i;

	i = 0;
	while (dst && dst[i])
	{
		free(dst[i]);
		i++;
	}
	free(dst);
}

int	ft_exit_handle(t_cmd *cmd, t_data *data, int status, char *input)
{
	status = ft_exit(cmd->args, data);
	if (status != 2)
	{
		free(input);
		ft_free_tab(cmd->args);
		free_all(data, status, NULL);
		exit(status);
	}
	return (status);
}

int	exec_builtins(t_cmd *cmd, t_data *data, char **env, char *input)
{
	int	status;

	status = 0;
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strncmp(cmd->args[0], "env", 3) == 0)
		status = ft_env(env);
	else if (ft_strncmp(cmd->args[0], "pwd", 3) == 0)
		status = ft_pwd();
	else if (ft_strncmp(cmd->args[0], "exit", 4) == 0)
		status = ft_exit_handle(cmd, data, status, input);
	else if (ft_strncmp(cmd->args[0], "echo", 4) == 0)
		status = ft_echo(cmd->args);
	else if (ft_strncmp(cmd->args[0], "cd", 2) == 0)
		status = ft_cd(cmd->args);
	else if (ft_strncmp(cmd->args[0], "export", 6) == 0)
		status = ft_export(cmd->args, env, data);
	else if (ft_strncmp(cmd->args[0], "unset", 5) == 0)
		status = ft_unset(cmd->args, env, data);
	else
	{
		printf("minishell: %s: command not found\n", input);
		status = 127;
	}
	return (status);
}
