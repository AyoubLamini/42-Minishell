#include "minishell_exec.h"

// static void restore_original_fd(t_exec *file_d)
// {
//     if (file_d->out != 1)
//         dup2(file_d->out, STDOUT_FILENO);
//     if (file_d->in != 0)
//         dup2(file_d->in, STDIN_FILENO);
// }

void execute(t_command *current, t_env **env_vars)
{
    // exit(0);
    t_exec file_d;
    t_command *command; 
    (void) env_vars;
    // int i = 0;
    file_d.in = 0;
    file_d.out = 1;
    int input_fd;
    input_fd = 0;
    
    command = current;
    if (!current)
        return ;
    while (command)
    {
        // if (current->redirection[0])
        //     handle_redirection(current, &file_d);
        piping(command, env_vars, &input_fd);
        command = command->next;
    }
    
    // restore_original_fd(&file_d);
}
           
   
