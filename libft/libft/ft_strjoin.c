/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/08 17:33:15 by hadia             #+#    #+#             */
/*   Updated: 2024/11/20 13:03:01 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*str;
	size_t	i_s1;
	size_t	i_s2;
	size_t	j;

	if (!s1 || !s2)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	i_s1 = 0;
	i_s2 = 0;
	j = 0;
	while (s1[i_s1])
		str[j++] = s1[i_s1++];
	j = i_s1;
	while (s2[i_s2])
		str[j++] = s2[i_s2++];
	str[j] = '\0';
	return (str);
}
/*int main(int argc, char **argv)
{
		if (argc != 3)
		{
				printf("Usage: %s <string1> <string2>\n", argv[0]);
				return (1);
		}

		const char *s1 = argv[1];
		const char *s2 = argv[2];

		char *result = ft_strjoin(s1, s2);

		if (result == NULL)
		{
				printf("Erreur d'allocation mémoire.\n");
				return (1);
		}

		printf("Résultat de la concaténation : '%s'\n", result);

		free(result);

		return (0);
}*/
