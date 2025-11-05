/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 19:19:11 by hadia             #+#    #+#             */
/*   Updated: 2024/11/20 11:32:40 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_empty_substr(void)
{
	char	*d;

	d = malloc(sizeof(char) * 1);
	if (!d)
		return (NULL);
	d[0] = '\0';
	return (d);
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*substr;
	size_t	i;
	size_t	j;

	if (!s)
		return (NULL);
	if (start >= (unsigned int)ft_strlen(s) || len == 0)
		return (ft_empty_substr());
	else if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	substr = malloc(sizeof(char) * (len + 1));
	if (!substr)
		return (NULL);
	i = 0;
	j = 0;
	while (s[i])
	{
		if (i >= start && j < len)
			substr[j++] = s[i];
		i++;
	}
	substr[j] = 0;
	return (substr);
}
/*int main(int argc, char **argv)
{

		if (argc != 4)
		{
				printf("Usage: %s <string> <start> <length>\n", argv[0]);
				return (1);
		}

		const char *input_str = argv[1];
		unsigned int start = (unsigned int)atoi(argv[2]);
		size_t len = (size_t)atoi(argv[3]);


		char *substr = ft_substr(input_str, start, len);

		if (substr == NULL)
		{
				printf("Erreur : impossible d'allouer de la mémoire.\n");
				return (1);
		}
		// Affichage des résultats
		printf("Chaîne d'origine : '%s'\n", input_str);
		printf("Sous-chaîne extraite (start=%u, len=%zu) : '%s'\n", start, len,
				substr);
		free(substr);

		return (0);
}*/
