#include "minishell_exec.h" 

void exit_status(int status, t_path *path)
{
    path->exit_status = status;
    return ;
}