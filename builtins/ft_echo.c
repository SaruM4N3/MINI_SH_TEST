/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebroue <leobroue@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:35:05 by lebroue           #+#    #+#             */
/*   Updated: 2025/11/05 01:27:49 by lebroue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <unistd.h>

static size_t	count_tab(char **str)
{
	size_t	i;

	i = 0;
	while (str && str[i])
		i++;
	return (i);
}

int	ft_echo(char **str)
{
	int		i;
	int		n_flag;
	size_t	count;

	i = 1;
	n_flag = 0;
	count = count_tab(str);
	if (count > 1 && str[1][0] == '-' && str[1][1] == 'n' && str[1][2] == '\0')
	{
		n_flag = 1;
		i++;
	}
	while (str[i])
	{
		write(1, str[i], ft_strlen(str[i]));
		if (str[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!n_flag)
		write(1, "\n", 1);
	return (0);
}
