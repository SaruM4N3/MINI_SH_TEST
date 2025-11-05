/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command_builder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 05:00:20 by hadia             #+#    #+#             */
/*   Updated: 2025/11/04 22:24:29 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	process_redirection_token(t_token **current,
		t_redir_type redir_type, t_cmd *cmd)
{
	char	*target_file;
	t_redir	*new_redir;

	target_file = (*current)->value;
	new_redir = create_redir(redir_type, target_file);
	if (new_redir)
	{
		if (redir_type == R_HEREDOC)
			handle_heredoc(new_redir);
		if (new_redir)
			add_redir_back(&(cmd->redirs), new_redir);
		if (!new_redir)
			return (0);
		*current = (*current)->next;
	}
	return (1);
}

int	build_redirection_token(t_token **current, t_cmd *cmd)
{
	t_redir_type	redir_type;

	if ((*current)->type == T_REDIR_IN)
		redir_type = R_IN;
	else if ((*current)->type == T_REDIR_OUT)
		redir_type = R_OUT_TRUNC;
	else if ((*current)->type == T_REDIR_APPEND)
		redir_type = R_OUT_APPEND;
	else if ((*current)->type == T_HEREDOC)
		redir_type = R_HEREDOC;
	else
		redir_type = R_IN;
	*current = (*current)->next;
	if (*current && (*current)->type == T_WORD)
	{
		process_redirection_token(current, redir_type, cmd);
	}
	return (1);
}

static char	*find_command_name(t_token *tokens)
{
	t_token	*current;
	int		after_redirection;

	if (!tokens)
		return (NULL);
	current = tokens;
	after_redirection = 0;
	while (current && current->type != T_PIPE && current->type != T_EOF)
	{
		if (current->type >= T_REDIR_IN && current->type <= T_HEREDOC)
		{
			after_redirection = 1;
		}
		else if (current->type == T_WORD)
		{
			if (!after_redirection)
				return (ft_strdup(current->value));
			after_redirection = 0;
		}
		current = current->next;
	}
	return (NULL);
}

static int	build_args_and_redirections(t_token **tokens, t_cmd *cmd)
{
	char	**args;
	int		arg_count;

	if (!tokens || !*tokens || !cmd)
		return (0);
	args = init_args_array(cmd->name, &arg_count);
	if (!args)
		return (0);
	arg_count = 1;
	if (!process_tokens(tokens, cmd, args, &arg_count))
		return (0);
	args[arg_count] = NULL;
	cmd->args = args;
	return (1);
}

t_cmd	*build_command(t_token **tokens)
{
	t_cmd	*cmd;
	t_token	*current;

	if (!tokens || !*tokens)
		return (NULL);
	cmd = create_cmd();
	if (!cmd)
		return (NULL);
	current = *tokens;
	cmd->name = find_command_name(current);
	if (!build_args_and_redirections(&current, cmd))
	{
		free_cmds(cmd);
		return (NULL);
	}
	*tokens = current;
	return (cmd);
}
