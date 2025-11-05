/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 05:00:48 by hadia             #+#    #+#             */
/*   Updated: 2025/11/01 05:21:36 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

t_cmd	*create_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
	{
		return (NULL);
	}
	cmd->name = NULL;
	cmd->args = NULL;
	cmd->redirs = NULL;
	cmd->next = NULL;
	return (cmd);
}

t_redir	*create_redir(t_redir_type type, const char *file)
{
	t_redir	*redir;

	if (!file)
		return (NULL);
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->type = type;
	redir->file = ft_strdup(file);
	if (!redir->file)
	{
		free(redir);
		return (NULL);
	}
	redir->next = NULL;
	return (redir);
}

void	add_redir_back(t_redir **redirs, t_redir *new_redir)
{
	t_redir	*current;

	if (!redirs || !new_redir)
		return ;
	if (!*redirs)
	{
		*redirs = new_redir;
		return ;
	}
	current = *redirs;
	while (current->next)
		current = current->next;
	current->next = new_redir;
}

void	free_args_on_error(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	*remove_quotes(const char *value, char quote_type)
{
	int		len;
	char	*result;
	int		i;

	if (!value)
		return (NULL);
	len = ft_strlen(value);
	if (len < 2 || value[0] != quote_type || value[len - 1] != quote_type)
		return (ft_strdup(value));
	result = malloc(len - 1);
	if (!result)
		return (NULL);
	i = 0;
	while (i < len - 2)
	{
		result[i] = value[i + 1];
		i++;
	}
	result[i] = '\0';
	return (result);
}
