/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_handler.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hadia <hadia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/01 05:00:34 by hadia             #+#    #+#             */
/*   Updated: 2025/11/01 05:16:45 by hadia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parsing.h"

static char	*generate_temp_filename(void)
{
	static int	heredoc_count = 0;
	char		*count_str;
	char		*filename;
	char		*temp;

	count_str = ft_itoa(heredoc_count++);
	temp = ft_strjoin("/tmp/.heredoc_", count_str);
	filename = ft_strjoin(temp, "_minishell");
	free(count_str);
	free(temp);
	return (filename);
}

static void	write_in_heredoc_file(int fd, const char *delimiter)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(fd, line, strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

void	handle_heredoc(t_redir *redir)
{
	int		fd;
	char	*temp_filename;
	char	*delimiter;

	if (!redir || !redir->file)
		return ;
	delimiter = ft_strdup(redir->file);
	temp_filename = generate_temp_filename();
	if (!temp_filename || !delimiter)
	{
		free(delimiter);
		return ;
	}
	fd = open(temp_filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
	{
		free(temp_filename);
		free(delimiter);
		return ;
	}
	write_in_heredoc_file(fd, delimiter);
	close(fd);
	free(redir->file);
	free(delimiter);
	redir->file = temp_filename;
}
