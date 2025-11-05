#ifndef MINISHELL_H
# define MINISHELL_H

# include "env.h"
# include "exec.h"
# include "minishell_signals.h"
# include "parsing.h"
# include "builtins.h"
# include "env.h"
# include <errno.h>
# include <fcntl.h> // open()
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h> // malloc, free
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h> // c

////////////ENV.C && LST_UTILS_ENV.C///////////

// int		add_back_env(t_env **envd);
// t_env	*ft_lstlast_env(t_env *envd);
// void	free_lst_env(t_env **envd, bool ext, int errcode);
// void	init_lst_env(t_env **envd, char **env);

// void		print_lst_env(t_env *envd);
// void		print_envp(char **envp);

// void		ft_free_tab_str(char **str);
// char		*envp_search(char **envp);
// int			path_check(char *path, int *ret);
// char		*strjoin_check_error(char *str);
// char		*get_path_in_paths_list(char **paths, int *ret, char *buffer,
// 				char *argv_cmd);
// char		*get_path(char **envp, char *argv_cmd, int *ret);
// char		*ft_strdup(const char *s);
// char		*get_unique_heredoc_path(void);
// char		*handle_heredoc(char *delimiter);
// void		apply_redirections(t_cmd *cmd);
// // char	**build_argv(t_pars *pars);
// void		free_argv(char **argv);
// // char	**build_envp_from_lst_env(t_env *env);
// void		free_envp(char **envp);
// void		update_envp(t_data *data);
// int			exec_cmd(t_cmd *cmds, t_data *data);
// int		exec_cmd(t_cmd *cmds);

// int			add_back_env(t_env **envd);
// t_env		*ft_lstlast_env(t_env *envd);
// void		free_lst_env(t_env **envd, bool ext, int errcode);
// void		init_lst_env(t_env **envd, char **env);

// void		print_lst_env(t_env *envd);
// void		print_envp(char **envp);

#endif /* MINISHELL_H */