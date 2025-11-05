/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 04:59:52 by hadia             #+#    #+#             */
/*   Updated: 2025/11/01 05:19:33 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

int	handle_word_token(t_args_builder *builder, const char *value)
{
	char	**new_args;

	if (builder->count >= builder->capacity - 1)
	{
		builder->capacity *= 2;
		new_args = ft_realloc(builder->args, sizeof(char *)
				* (builder->capacity));
		if (!new_args)
			return (0);
		builder->args = new_args;
	}
	builder->args[builder->count] = ft_strdup(value);
	if (!builder->args[builder->count])
		return (0);
	builder->count++;
	return (1);
}

int	handle_token(t_token **current, t_cmd *cmd, t_args_builder *builder)
{
	if ((*current)->type == T_WORD)
	{
		if (cmd->name && builder->count == 1 && strcmp((*current)->value,
				cmd->name) == 0)
			*current = (*current)->next;
		else
		{
			if (!handle_word_token(builder, (*current)->value))
				return (0);
			*current = (*current)->next;
		}
	}
	else if ((*current)->type >= T_REDIR_IN && (*current)->type <= T_HEREDOC)
	{
		if (!build_redirection_token(current, cmd))
			return (0);
	}
	else
		*current = (*current)->next;
	return (1);
}

int	process_tokens(t_token **tokens, t_cmd *cmd, char **args, int *arg_count)
{
	t_token			*current;
	t_args_builder	builder;

	current = *tokens;
	builder.args = args;
	builder.count = *arg_count;
	builder.capacity = 10;
	while (current && current->type != T_PIPE && current->type != T_EOF)
	{
		if (!handle_token(&current, cmd, &builder))
		{
			free_args_on_error(args);
			return (0);
		}
	}
	*arg_count = builder.count;
	*tokens = current;
	return (1);
}

char	**init_args_array(const char *cmd_name, int *capacity)
{
	char	**args;

	*capacity = 10;
	args = malloc(sizeof(char *) * (*capacity));
	if (!args)
		return (NULL);
	if (cmd_name != NULL)
	{
		args[0] = ft_strdup(cmd_name);
		if (!args[0])
		{
			free(args);
			return (NULL);
		}
	}
	else
		args[0] = NULL;
	return (args);
}

void	free_cmds(t_cmd *cmds)
{
	t_cmd	*temp;
	t_cmd	*current;

	current = cmds;
	while (current)
	{
		temp = current->next;
		free(current->name);
		free_args_on_error(current->args);
		free_redirs(current->redirs);
		free(current);
		current = temp;
	}
}
