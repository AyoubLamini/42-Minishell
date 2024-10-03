#include "minishell_exec.h"


static void reset_fd(t_path *path)
{
	dup2(path->fd_in, STDIN_FILENO);
	dup2(path->fd_out, STDOUT_FILENO);
}
static int is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ex_strcmp("cd", cmd) == 0)
		return (1);
	else if (ex_strcmp("pwd", cmd) == 0)
		return (1);
	else if (ex_strcmp("echo", cmd) == 0)
		return (1);
	else if (ex_strcmp("export", cmd) == 0)
		return (1);
	else if (ex_strcmp("env", cmd) == 0)
		return (1);
	else if (ex_strcmp("unset", cmd) == 0)
		return (1);
	else if (ex_strcmp("exit", cmd) == 0)
		return (1);
	return (0);

}

static void exec_builtin(t_command *current, t_env **env_vars, t_path *path)
{
	if (current->redirection[0])
			handle_redirection(current, path);
	check_command(current, env_vars, path);
	reset_fd(path);
}



void execute(t_command *command, t_env **env_vars, t_path *path)
{
	t_command   *current;

	int input_fd;

	input_fd = -1;
	current = command;
	if (!current)
		return ;

	while (current)
	{
		handle_herdoc(current, path, env_vars);
		if (!current->cmd[0])
		{
			if (current->redirection[0])
			{
				handle_redirection(current, path);
				reset_fd(path);
			}		
		}
		else
		{
			if (is_builtin(current->cmd[0]) && !current->next)
			{
				exec_builtin(current, env_vars, path);
				return ;
			}
			piping(current, env_vars, &input_fd, path);
		}
		current = current->next;
	}
	while (waitpid(-1, &path->exit_status, 0) != -1 && errno != ECHILD)
		;
}
		   
   