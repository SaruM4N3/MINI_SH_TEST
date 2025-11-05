/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator_handler.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 05:00:03 by hadia             #+#    #+#             */
/*   Updated: 2025/11/01 05:17:15 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static t_token_type	handle_redirection_in(const char *str, int *i)
{
	if (str[*i + 1] == '<')
	{
		(*i) += 2;
		return (T_HEREDOC);
	}
	else
	{
		(*i)++;
		return (T_REDIR_IN);
	}
}

static t_token_type	handle_redirection_out(const char *str, int *i)
{
	if (str[*i + 1] == '>')
	{
		(*i) += 2;
		return (T_REDIR_APPEND);
	}
	else
	{
		(*i)++;
		return (T_REDIR_OUT);
	}
}

static t_token_type	get_redirection_type(const char *str, int *i)
{
	if (str[*i] == '<')
	{
		return (handle_redirection_in(str, i));
	}
	else if (str[*i] == '>')
	{
		return (handle_redirection_out(str, i));
	}
	(*i)++;
	return (T_WORD);
}

/*
 * Détermine le type d'opérateur et avance l'index
 */
t_token_type	get_operator_type(const char *str, int *i)
{
	if (str[*i] == '|')
	{
		(*i)++;
		return (T_PIPE);
	}
	else if (str[*i] == '<' || str[*i] == '>')
	{
		return (get_redirection_type(str, i));
	}
	(*i)++;
	return (T_WORD);
}

/*
 * Vérifie si un caractère est le début d'un opérateur
 */
int	is_operator_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
