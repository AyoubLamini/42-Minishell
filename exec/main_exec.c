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
void check_command(t_command *command, t_env **env_vars)
{
    if (ex_strcmp("cd", command->cmd[0]) == 0)
        cd(command, *env_vars);
    else if (ex_strcmp("echo" ,command->cmd[0]) == 0)
        echo(command);
    else if (ex_strcmp("export" ,command->cmd[0]) == 0)
        export(command, env_vars);
    else if (ex_strcmp("env" ,command->cmd[0]) == 0)
        env(*env_vars);
    else if (ex_strcmp("unset" ,command->cmd[0]) == 0)
        unset(command, env_vars);
    // else if (ex_strcmp("exit" ,command->cmd[0]) == 0)
    //     exit(command);
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

void execute(t_command *command, t_env **env_vars)
{
    t_exec file_d;
    t_command *current; 

    file_d.in = 0;
    file_d.out = 1;
    int input_fd;
    input_fd = 0;
    
    current = command;
    if (!current)
        return ;
    while (current)
    {
        input_fd = 0;
        // if (current->redirection[0])
        //     handle_redirection(current, &file_d);
        piping(current, env_vars, &input_fd); // wc - l
        current = current->next;
    }
    restore_original_fd(&file_d);
}
           
   
