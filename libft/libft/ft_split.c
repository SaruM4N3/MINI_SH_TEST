/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:37:14 by hadia             #+#    #+#             */
/*   Updated: 2024/11/20 16:13:16 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/*fonction compte nombre mot dans s*/
static int	ft_count_word(char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

/*fonction free tab si echec allocation de ft_substr*/
static void	ft_free_tab(char **tab)
{
	int	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
}

/*fonction complete tab a l'aide de substr*/
static int	write_split(char **tab, char const *s, char c)
{
	int	start;
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		start = i;
		while (s[i] && s[i] != c)
			i++;
		if (i > start)
		{
			tab[j] = ft_substr(s, start, i - start);
			if (!tab[j])
			{
				ft_free_tab(tab);
				return (0);
			}
			j++;
		}
	}
	return (1);
}

/*fonction check securite malloc, gere overflow*/
char	**ft_split(char const *s, char c)
{
	size_t	words;
	char	**tab;

	if (!s)
		return (NULL);
	words = ft_count_word((char *)s, c);
	tab = malloc(sizeof(char *) * (words + 1));
	if (!tab)
		return (NULL);
	if (!write_split(tab, s, c))
	{
		return (NULL);
	}
	tab[words] = NULL;
	return (tab);
}
/*int main(int argc, char **argv)
{
		if (argc != 3)
		{
				printf("Usage: %s <string> <delimiter>\n", argv[0]);
				return (1);
		}

		char *string = argv[1];
		char delimiter = argv[2][0];


		char **result = ft_split(string, delimiter);

		if (!result)
		{
				printf("Error: Memory allocation failed.\n");
				return (1);
		}


		printf("Split result:\n");
		for (int i = 0; result[i] != NULL; i++)
		{
				printf("[%d]: %s\n", i, result[i]);
		}


		ft_free_tab(result);

		return (0);
}*/
