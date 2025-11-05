
#ifndef ENV_H
# define ENV_H

# include <stdbool.h>
# include <stdlib.h>

// Forward declaration
typedef struct s_data	t_data;

typedef struct s_env
{
	char *key;          // HOME
	char *value;        // /home/lebroue
	struct s_env *next; // On passe a la variable d'env d'apres

}						t_env;

// Structure principale de données
typedef struct s_cmd	t_cmd;
typedef struct s_data
{
	char				**dst;
	t_env				*env;
	char				**envp;
	int					errcode;
	int					i;
	int					saved_stdin;
	int					saved_stdout;
	t_cmd *cmds; // t_cmd * mais évite dépendance circulaire // mette t_cmd
}						t_data;

////////////ENV.C && LST_UTILS_ENV.C///////////
void					init_data(t_data *data, t_env **envd, char **dst);
void					init_envp(t_data *data);
int						add_back_env(t_env **envd);
t_env					*ft_lstlast_env(t_env *envd);
void					free_lst_env(t_env **envd, bool ext, int errcode);
void					init_lst_env(t_env **envd, char **env);

void					print_lst_env(t_env *envd);
void					print_envp(char **envp);

int						free_all(t_data *data, int errcode, char *str);

void					add_or_update_env(t_env **env, char *str);

#endif