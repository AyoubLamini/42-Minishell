#include "../includes/minishell_exec.h"
void reset_fd(t_path *path)
{
    dup2(path->fd_in, STDIN_FILENO);
    dup2(path->fd_out, STDOUT_FILENO);
}
int is_builtin(char *cmd)
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