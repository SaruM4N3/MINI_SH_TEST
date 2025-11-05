/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:02:14 by hadia             #+#    #+#             */
/*   Updated: 2024/11/20 13:07:00 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_size(int nb)
{
	size_t	size;
	long	num;

	num = nb;
	size = 0;
	if (num < 0)
	{
		num = -num;
		size = 1;
	}
	if (num == 0)
	{
		size = 1;
	}
	else
	{
		while (num)
		{
			num = num / 10;
			size++;
		}
	}
	return (size);
}

char	*ft_itoa(int n)
{
	long	num;
	size_t	size;
	char	*str;
	size_t	is_neg;

	is_neg = 0;
	num = n;
	size = ft_size(num);
	str = malloc(sizeof(char) * (size + 1));
	if (!str)
		return (NULL);
	if (num < 0)
	{
		num = -num;
		str[0] = '-';
		is_neg = 1;
	}
	str[size] = '\0';
	while (size > is_neg)
	{
		str[size - 1] = num % 10 + '0';
		num = num / 10;
		size--;
	}
	return (str);
}
