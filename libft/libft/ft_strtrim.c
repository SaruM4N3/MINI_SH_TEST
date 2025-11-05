/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 12:07:47 by hadia             #+#    #+#             */
/*   Updated: 2024/11/20 13:37:38 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_check(char c, const char *set)
{
	int	i;

	i = 0;
	while (set[i])
	{
		if (set[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*strim;
	int		i;
	int		i_start;
	int		i_end;

	if (!s1 || !set)
		return (NULL);
	i_start = 0;
	while (s1[i_start] && ft_check(s1[i_start], set))
		i_start++;
	i_end = ft_strlen(s1);
	while (i_end > i_start && ft_check(s1[i_end - 1], set))
		i_end--;
	strim = malloc(sizeof(char) * (i_end - i_start + 1));
	if (!strim)
		return (NULL);
	i = 0;
	while (i_start < i_end)
	{
		strim[i] = s1[i_start];
		i++;
		i_start++;
	}
	strim[i] = '\0';
	return (strim);
}
/*int main(int argc, char **argv)
{

		if (argc != 3)
		{
				printf("Usage: %s <string> <set_of_chars>\n", argv[0]);
				return (1);
		}
		const char *input_str = argv[1];
		const char *set_of_chars = argv[2];

		char *trimmed_str = ft_strtrim(input_str, set_of_chars);

		if (trimmed_str == NULL)
		{
				printf("Erreur: impossible d'allouer de la mémoire.\n");
				return (1);
		}
		printf("Chaîne d'origine : '%s'\n", input_str);
		printf("Caractères à retirer : '%s'\n", set_of_chars);
		printf("Chaîne après trim : '%s'\n", trimmed_str);
		free(trimmed_str);

		return (0);
}*/
