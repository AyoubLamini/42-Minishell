#include "minishell_exec.h"

static void restore_original_fd(t_exec *file_d)
{
    if (file_d->out != 1)
        dup2(file_d->out, STDOUT_FILENO);
    if (file_d->in != 0)
        dup2(file_d->in, STDIN_FILENO);
}

static int is_builtin(char *cmd)
{
    if (ex_strcmp("cd", cmd) == 0)
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
void execute(t_command *command, t_env **env_vars)
{
    t_exec      file_d;
    t_command   *current; 
    t_path path;

    path.exit_status = 0;
    file_d.in = 0;
    file_d.out = 1;
    int input_fd;
    input_fd = 0;
    
    current = command;
    if (!current)
        return ;
    while (current)
    {
        if (current->redirection[0])
            handle_redirection(current, &file_d);
        if (is_builtin(current->cmd[0]) && !current->next) 
            check_command(current, env_vars, &path);
        else
            piping(current, env_vars, &input_fd, &file_d, &path);
        current = current->next;
        restore_original_fd(&file_d);
    }
}
           
   
