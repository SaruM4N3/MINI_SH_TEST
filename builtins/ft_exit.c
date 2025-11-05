/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebroue <leobroue@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:16:36 by lebroue           #+#    #+#             */
/*   Updated: 2025/11/05 04:21:55 by lebroue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "exec.h"

int	ft_atoi(const char *str)
{
	int		i;
	int		sign;
	long	result;

	i = 0;
	sign = 1;
	result = 0;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		result = result * 10 + (str[i] - '0');
		if (sign * result > INT_MAX || sign * result < INT_MIN)
			return (0);
		i++;
	}
	return (sign * result);
}

bool	isnumber(const char *str)
{
	int			i;
	int			nb_start;
	const char	*nb;
	int			sign;

	i = 0;
	if (!str || !*str)
		return (false);
	sign = get_sign(str, &i);
	nb_start = i;
	if (!check_if_only_digits(str, nb_start))
		return (false);
	nb = &str[nb_start];
	nb = skip_zero_at_begining(nb);
	if (!check_len_and_limits(nb, sign))
		return (false);
	return (true);
}

int	ft_exit(char **args, t_data *data)
{
	long long	nb;

	nb = 0;
	write(1, "exit\n", 5);
	if (args[1])
	{
		if (!isnumber(args[1]))
		{
			write(2, "minishell: exit: ", 18);
			write(2, args[1], ft_strlen(args[1]));
			write(2, ": numeric argument required\n", 27);
			return (-12);
		}
		if (args[2])
		{
			write(2, "minishell: exit: too many arguments\n", 37);
			nb = 1;
		}
		else
			nb = ft_atoi(args[1]);
	}
	free_envp(data->envp);
	free_cmds(data->cmds);
	free_lst_env(&data->env, false, 0);
	exit((int)(nb % 256));
}
