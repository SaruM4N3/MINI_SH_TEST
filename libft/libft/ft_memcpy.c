/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 16:54:44 by hadia             #+#    #+#             */
/*   Updated: 2024/11/20 11:43:52 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	const unsigned char	*c;
	unsigned char		*d;
	size_t				i;

	if (!dest && !src)
		return (NULL);
	d = (unsigned char *)dest;
	c = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = c[i];
		i++;
	}
	return (dest);
}
/*int main()
{
	char str[] = "abcd";
	char dst[] = "efgh";

	ft_memcpy(dst, str, 3);
	printf("%s\n", dst);
}*/
