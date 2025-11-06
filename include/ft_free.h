#ifndef FT_FREE_H
# define FT_FREE_H

#include<env.h>
#include<parsing.h>

//EXEC/ENV
void	free_argv(char **argv);
void	free_envp(char **envp);
void	ft_free_tab_str(char **str);
void	free_lst_env(t_env **envd, bool ext, int errcode);
int		free_all(t_data *data, int errcode, char *str);

//PARSING
// Libération mémoire
void				free_tokens(t_token *head);
void				free_cmds(t_cmd *cmds);
void				free_redirs(t_redir *redirs);
void				free_args(char **args);
void				free_args_on_error(char **args);

#endif