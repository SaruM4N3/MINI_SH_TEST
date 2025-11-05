/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebroue <leobroue@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:57:14 by lebroue           #+#    #+#             */
/*   Updated: 2025/11/05 04:10:02 by lebroue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static bool	identifier_correct(char *str)
{
	int		i;
	char	*eq_pos;
	int		len;

	eq_pos = ft_strchr(str, '=');
	i = 1;
	if (eq_pos)
		len = eq_pos - str;
	else
		len = ft_strlen(str);
	if (len == 0)
		return (false);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (false);
	while (i < len)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (false);
		i++;
	}
	return (true);
}

static int	handle_no_args(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		write(1, "export ", 7);
		write(1, env[i], ft_strlen(env[i]));
		write(1, "\n", 1);
		i++;
	}
	return (0);
}

static int	process_single_arg(char *arg, t_data *data)
{
	char	*eq_pos;
	char	*key_only;
	int		exit_flag;

	eq_pos = ft_strchr(arg, '=');
	exit_flag = 0;
	if (eq_pos != NULL)
		key_only = ft_strndup(arg, eq_pos - arg);
	else
		key_only = ft_strdup(arg);
	if (!identifier_correct(key_only))
	{
		write(2, "export: `", 9);
		write(2, arg, ft_strlen(arg));
		write(2, "': not a valid identifier\n", 28);
		exit_flag = 1;
	}
	else
	{
		add_or_update_env(&data->env, arg);
	}
	free(key_only);
	return (exit_flag);
}

int	ft_export(char **args, char **env, t_data *data)
{
	int	i;
	int	exit_code;

	i = 0;
	exit_code = 0;
	if (!args[1])
		return (handle_no_args(env));
	i = 1;
	while (args[i] != NULL)
	{
		if (process_single_arg(args[i], data))
			exit_code = 1;
		i++;
	}
	update_envp(data);
	return (exit_code);
}
