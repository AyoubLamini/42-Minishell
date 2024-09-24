#include "minishell_exec.h"


static void reset_fd(t_exec *file_d)
{
	dup2(file_d->in, STDIN_FILENO);
	dup2(file_d->out, STDOUT_FILENO);
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

static void exec_builtin(t_command *current, t_env **env_vars, t_exec *file_d, t_path *path)
{
	if (current->redirection[0])
			handle_redirection(current, file_d, path);
	check_command(current, env_vars, path);
	reset_fd(file_d);
}



void execute(t_command *command, t_env **env_vars, t_path *path)
{
	t_exec      file_d;
	t_command   *current; 
	int input_fd;
	file_d.in = 0;
	file_d.out = 1;
	path->is_forked = 0;
	input_fd = -1;
	
	current = command;
	if (!current)
		return ;
	while (current)
	{
		if (!current->cmd[0])
		{
			if (current->redirection[0])
			{
				handle_redirection(current, &file_d, path);
				reset_fd(&file_d);
			}		
		}
		else
		{
			if (is_builtin(current->cmd[0]) && !current->next)
			{
				exec_builtin(current, env_vars, &file_d, path);
				return ;
			}
			piping(current, env_vars, &input_fd, &file_d, path);
		}
		current = current->next;
	}
	while (waitpid(-1, &path->exit_status, 0) != -1 && errno != ECHILD)
		;
}
		   
   