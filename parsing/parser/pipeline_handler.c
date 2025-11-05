/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 05:00:39 by hadia             #+#    #+#             */
/*   Updated: 2025/11/04 22:29:22 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

void	free_redirs(t_redir *redirs)
{
	t_redir	*current;
	t_redir	*temp;

	current = redirs;
	while (current)
	{
		temp = current->next;
		free(current->file);
		free(current);
		current = temp;
	}
}

static void	build_pipeline(t_token **current_token, t_cmd **current_cmd)
{
	t_cmd	*new_cmd;

	while (current_token && *current_token && (*current_token)->type != T_EOF)
	{
		if ((*current_token)->type == T_PIPE)
		{
			*current_token = (*current_token)->next;
			new_cmd = build_command(current_token);
			if (new_cmd)
			{
				(*current_cmd)->next = new_cmd;
				*current_cmd = new_cmd;
			}
		}
		else
			*current_token = (*current_token)->next;
	}
}

t_cmd	*handle_pipeline(t_token *tokens)
{
	t_cmd	*first_cmd;
	t_cmd	*current_cmd;
	t_token	*current_token;

	if (!tokens)
		return (NULL);
	current_token = tokens;
	first_cmd = build_command(&current_token);
	if (!first_cmd)
		return (NULL);
	current_cmd = first_cmd;
	build_pipeline(&current_token, &current_cmd);
	return (first_cmd);
}
