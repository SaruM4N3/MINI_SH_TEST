#ifndef EXEC_H
# define EXEC_H

# include <limits.h>

# include "minishell.h"
# include "parsing.h"

void	print_lst_env(t_env *envd);
void	print_envp(char **envp);

void	ft_free_tab_str(char **str);
char	*envp_search(char **envp);
int		path_check(char *path, int *ret);
char	*strjoin_check_error(char *str);
char	*get_path_in_paths_list(char **paths, int *ret, char *buffer,
			char *argv_cmd);
char	*get_path(char **envp, char *argv_cmd, int *ret);
char	*ft_strdup(const char *s);
char	*get_unique_heredoc_path(void);
// char	*handle_heredoc(char *delimiter);
int		apply_redirections(t_cmd *cmd);
// char	**build_argv(t_pars *pars);
void	free_argv(char **argv);
// char	**build_envp_from_lst_env(t_env *env);
void	free_envp(char **envp);
void	update_envp(t_data *data);
int		exec_cmd(t_data *data, char *input);



void	reset_std_in_out(t_data *data);


void	save_std_in_out(t_data *data);


// #include "env.h"
////PATH UTILS///
// void	ft_free_tab_str(char **str);

// char	*envp_search(char **envp);

// int	path_check(char *path, int *ret);

// char	*strjoin_check_error(char *str);

// //GET PATH///////

// char	*get_path_in_paths_list(char **paths, int *ret, char *buffer,
// char *argv_cmd);

// char	*get_path(char **envp, char *argv_cmd, int *ret);

//
//////////////////////////////////////////////////////////////////////////////////////////////

// char *get_unique_heredoc_path(void);

// char *handle_heredoc(char *delimiter);

// void apply_redirections(t_exec *exec);

// char **build_argv(t_pars *pars);

// void free_argv(char **argv);

// char **build_envp_from_lst_env(t_env *env);

// void free_envp(char **envp);

// void update_envp(t_data *data);

// int exec_cmd(t_data *data);

#endif
