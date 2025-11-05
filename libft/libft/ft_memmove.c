/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/05 14:26:35 by hadia             #+#    #+#             */
/*   Updated: 2024/11/20 11:47:02 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*c;
	unsigned char	*d;
	size_t			i;

	if (!dest && !src)
		return (NULL);
	c = (unsigned char *)src;
	d = (unsigned char *)dest;
	if (dest < src)
	{
		i = -1;
		while (++i < n)
			d[i] = c[i];
	}
	else
	{
		i = n;
		while (i > 0)
		{
			d[i - 1] = c[i - 1];
			i--;
		}
	}
	return (dest);
}
