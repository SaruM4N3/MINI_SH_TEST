/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebroue <leobroue@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/05 02:09:37 by lebroue           #+#    #+#             */
/*   Updated: 2025/11/05 02:23:28 by lebroue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static void	split_key_value(const char *str, char **key_out, char **value_out)
{
	char	*eq_pos;

	eq_pos = ft_strchr(str, '=');
	if (eq_pos)
	{
		*key_out = ft_strndup(str, eq_pos - str);
		*value_out = ft_strdup(eq_pos + 1);
	}
	else
	{
		*key_out = ft_strdup(str);
		*value_out = NULL;
	}
}

static bool	try_update_existing_env(t_env *env, const char *key, char *value)
{
	t_env	*tmp;

	tmp = env;
	while (tmp)
	{
		if (!ft_strcmp(tmp->key, key))
		{
			if (value)
			{
				free(tmp->value);
				tmp->value = value;
			}
			else
			{
				free((char *)key);
			}
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}

static void	add_new_env_node(t_env **env, char *key, char *value)
{
	t_env	*new_node;
	t_env	*tmp;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
	{
		free(key);
		if (value)
			free(value);
		return ;
	}
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	if (!*env)
		*env = new_node;
	else
	{
		tmp = *env;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

void	add_or_update_env(t_env **env, char *str)
{
	char	*key;
	char	*value;
	bool	updated;

	split_key_value(str, &key, &value);
	updated = try_update_existing_env(*env, key, value);
	if (updated)
		return ;
	add_new_env_node(env, key, value);
}
