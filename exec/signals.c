#include "minishell_exec.h"

void    sigint_handler(int sig)
{
	(void)sig;
	if (waitpid(-1, &sig, WNOHANG) == 0) // 
		return ;
	printf("\n");
	//  prompt_pwd();
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	// set_exit_status(1);
	// g_last_signal = 69;
}

void setup_signals(t_path *path, int action)
{
	if (action == SET_SIG)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (action == UNSET_SIG)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
	}
    exit_status(1, path);
	return ;
}
