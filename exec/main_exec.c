#include "minishell_exec.h"

void commands(t_command *command)
{

    int i;
    int j;

    
    i = 0;
    j = 0;
    char **ptr;

    ptr = ex_split(getenv("PATH"), ':');
    while (ptr[i])
    {
        ptr[i] = ex_strjoin(ptr[i], "/");
        ptr[i] = ex_strjoin(ptr[i], command->cmd[0]);
        if (access(ptr[i], X_OK) == 0)
            execute_command(command, ptr[i]);
        i++;
    }
}

void check_command(t_command *command)
{
    if (ex_strcmp("cd", command->cmd[0]) == 0)
        cd(command);
    else if (ex_strcmp("echo" ,command->cmd[0]) == 0)
        echo(command);
    else if (ex_strcmp("env" ,command->cmd[0]) == 0)
        env(command);
    else
       commands(command);
}
static void restore_original_fd(t_exec *file_d)
{
    if (file_d->out != 1)
        dup2(file_d->out, STDOUT_FILENO);
    if (file_d->in != 0)
        dup2(file_d->in, STDIN_FILENO);
}

void execute(t_command *command)
{
    t_exec file_d; 
    file_d.in = 0;
    file_d.out = 1; 
    if (command->redirection[0])
        handle_redirection(command, &file_d);
    check_command(command);
    restore_original_fd(&file_d);
}
           
   
