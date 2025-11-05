#ifndef MINISHELL_SIGNALS_H
# define MINISHELL_SIGNALS_H

# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include "../libft/libft/libft.h"

extern int g_exit_status;
extern volatile sig_atomic_t g_interrupted;

// /* **********************************SIGNALS**************************************** */
void setup_sigint(void);
void setup_sigquit(void);
void handle_sigint(int sig);
void handle_signals(void);

char    *reader(void);

#endif /* MINISHELL_SIGNALS_H */