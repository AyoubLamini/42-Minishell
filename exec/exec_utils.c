#include "../includes/minishell_exec.h"

void exit_status(int status, t_path *path)
{
    path->exit_status = status;
    return ;
}

void set_last_arg(t_command *command, t_env **env_vars)
{
    t_command *current;

    int i = 0;
    current = command;
    if (!current || !current->cmd[0])
        return ;
    while (current->cmd[i])
            i++;
    update_var(*env_vars, "_", ft_strdup(current->cmd[i - 1]));
}