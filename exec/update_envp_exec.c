/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_envp_exec.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebroue <leobroue@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/28 16:41:44 by lebroue           #+#    #+#             */
/*   Updated: 2025/11/05 19:33:44 by lebroue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env.h"
#include "exec.h"
#include "parsing.h"

int	count_node(t_env *tmp)
{
	int	count;

	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

int	ft_strjoin_checker_envp(char *buffer, char **envp, int i)
{
	if (!buffer)
	{
		while (i >= 0)
		{
			free(envp[i]);
			i--;
		}
		free(envp);
		return (0);
	}
	return (1);
}
char	**build_envp_tab_from_lst_env(t_env *env)
{
	t_env	*curr;
	int		i;
	char	**envp;
	char	*key_equal;

	i = 0;
	curr = env;
	envp = malloc(sizeof(char *) * (count_node(curr) + 1));
	if (!envp)
		return (NULL);
	while (curr)
	{
		// si key existe et que value vault NULL
		if (curr->key && curr->value == NULL)
			envp[i] = ft_strdup(curr->key); // dsddsddsdsds
		else if (curr->value != NULL)
		{
			free(envp[i]);
			envp[i] = ft_strjoin(curr->key, "=");
		}
		// printf("res 1: '%s'\n", envp[i]);
		if (!ft_strjoin_checker_envp(envp[i], envp, i))
			return (NULL);
		// printf("res 2: '%s'\n", envp[i]);
		key_equal = envp[i];
		// printf("%s\n", envp[i]);
		if (curr->value != NULL)
		
		{
			free(envp[i]);
			envp[i] = ft_strjoin(key_equal, curr->value); //
		}
		printf("res 3: '%s'\n", envp[i]);
		if (!ft_strjoin_checker_envp(envp[i], envp, i))
			return (NULL);
		// printf("OKKKKKKKKKKKKKKKKKKKKKKKK 3 \n");
		printf("Res final '%s'\n", envp[i]);
		curr = curr->next;
		i++;
	}
	envp[i] = NULL;
	return (envp);
}

// char	**build_envp_tab_from_lst_env(t_env *env)
// {
// 	// int		count;
// 	// t_env	*tmp;
// 	// char	**envp;
// 	// int		i;
// 	// char	*key_equal;

// 	// count = 0;
// 	// tmp = env;
// 	// i = 0;
// 	// count = count_node(tmp, count);
// 	// envp = malloc(sizeof(char *) * (count + 1));
// 	// if (!envp)
// 	// 	return (NULL);
// 	// tmp = env;
// 	// while (tmp)
// 	// {
// 	// 	envp[i] = ft_strjoin(tmp->key, "=");
// 	// 	if (!ft_strjoin_checker_envp(envp[i], envp, i))
// 	// 		return (NULL);
// 	// 	key_equal = envp[i];
// 	// 	envp[i] = ft_strjoin(key_equal, tmp->value);
// 	// 	free(key_equal);
// 	// 	if (!ft_strjoin_checker_envp(envp[i], envp, i))
// 	// 		return (NULL);
// 	// 	tmp = tmp->next;
// 	// 	i++;
// 	// }
// 	// envp[i] = NULL;
// 	return (join_key_equal_and_value(env));
// }

void	update_envp(t_data *data)
{
	if (data->envp)
		free_envp(data->envp);
	data->envp = build_envp_tab_from_lst_env(data->env);
	// printf("data envp: %p\n", data->envp);
}
