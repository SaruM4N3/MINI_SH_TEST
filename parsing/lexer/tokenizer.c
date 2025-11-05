/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 05:00:13 by hadia             #+#    #+#             */
/*   Updated: 2025/11/01 05:34:48 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static int	process_token(const char *line, int *i, t_token **tokens)
{
	int	start;

	start = *i;
	if (is_operator_char(line[*i]) || line[*i] == '"' || line[*i] == '\'')
	{
		if (!handle_operator_and_quote(i, &start, line, tokens))
			return (0);
	}
	else
	{
		while (line[*i] && !ft_isspace(line[*i]) && !is_operator_char(line[*i]))
			(*i)++;
		if (!tokenize_word(&start, i, tokens, line))
			return (0);
	}
	return (1);
}

t_token	*tokenize_line(const char *line)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		while (line[i] && ft_isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		if (!process_token(line, &i, &tokens))
			return (NULL);
	}
	return (tokens);
}

t_token	*create_token(t_token_type type, const char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	if (value)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

void	free_tokens(t_token *tokens)
{
	t_token	*temp;
	t_token	*current;

	current = tokens;
	while (current)
	{
		temp = current->next;
		free(current->value);
		free(current);
		current = temp;
	}
}

void	add_token_back(t_token **tokens, t_token *new_token)
{
	t_token	*current;

	if (!tokens || !new_token)
		return ;
	if (!*tokens)
	{
		*tokens = new_token;
		return ;
	}
	current = *tokens;
	while (current->next)
		current = current->next;
	current->next = new_token;
}
