/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lebroue <leobroue@student.42lyon.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/30 19:38:29 by lebroue           #+#    #+#             */
/*   Updated: 2025/11/05 01:37:37 by lebroue          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

bool	check_to_many_arguments(char **args)
{
	if (args[1])
	{
		if (args[2])
		{
			write(2, "cd: too many arguments\n", 24);
			return (true);
		}
	}
	return (false);
}

bool	path_checker_limit(char *path, size_t limit)
{
	if (ft_strlen(path) > limit)
	{
		write(2, "cd: File name too long\n", 24);
		return (true);
	}
	return (false);
}

bool	check_chdir(int ret)
{
	if (ret == -1)
	{
		perror("cd");
		return (true);
	}
	return (false);
}

int	ft_cd(char **args)
{
	char	*path;
	int		ret;
	size_t	limit;

	limit = 255;
	if (check_to_many_arguments(args) == true)
		return (1);
	if (!args[1] || (args[1][0] == '~' && (args[1][1] == '\0'
		|| args[1][1] == '/')))
	{
		path = getenv("HOME");
		if (!path)
		{
			write(2, "cd: HOME not set\n", 17);
			return (1);
		}
	}
	else
		path = ft_strdup(args[1]);
	if (path_checker_limit(path, limit) == true)
		return (1);
	ret = chdir(path);
	if (check_chdir(ret) == true)
		return (1);
	return (0);
}
