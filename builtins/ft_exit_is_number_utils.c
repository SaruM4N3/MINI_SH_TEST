/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit_is_number_utils.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebroue <leobroue@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:18:02 by lebroue           #+#    #+#             */
/*   Updated: 2025/11/05 01:36:07 by lebroue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "builtins.h"

int	get_sign(const char *str, int *i)
{
	int	sign;

	sign = 1;
	if (str[*i] == '+' || str[*i] == '-')
	{
		if (str[*i] == '-')
			sign = -1;
		(*i)++;
	}
	return (sign);
}

bool	check_if_only_digits(const char *str, int start)
{
	int	i;

	i = start;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

const char	*skip_zero_at_begining(const char *nb)
{
	while (*nb == '0' && nb[1] != '\0')
		nb++;
	return (nb);
}

bool	check_len_and_limits(const char *nb, int sign)
{
	int	nb_len;

	nb_len = 0;
	while (ft_isdigit(nb[nb_len]))
		nb_len++;
	if (nb_len > 19)
		return (false);
	if (nb_len == 19)
	{
		if (sign == 1 && ft_strncmp(nb, INT64_MAX_STR, 19) > 0)
			return (false);
		if (sign == -1 && ft_strncmp(nb, INT64_MIN_STR, 19) > 0)
			return (false);
	}
	return (true);
}
