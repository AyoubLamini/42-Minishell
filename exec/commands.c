#include "minishell_exec.h"
static void commands(t_command *command, char** envp)
{
    char **ptr;
    int i;
    int j;

    
    i = 0;
    j = 0;
    ptr = ex_split(getenv("PATH"), ':');
    while (ptr[i])
    {
        ptr[i] = ex_strjoin(ptr[i], "/");
        ptr[i] = ex_strjoin(ptr[i], command->cmd[0]);
        if (access(ptr[i], X_OK) == 0)
        {
            if (execve(ptr[i], command->cmd, envp) == -1)
            {
                perror("execve"); 
                exit(1);
            }
        } 
        i++;
    }
    print_error(command->cmd[0], NULL, "command not found");
    exit(1);
}
static char **envp_array(t_env *vars)
{
    t_env *tmp;
    char **envp;
    int i;
    char *str;

    i = 0;
    tmp = vars;
    envp = (char **)malloc(sizeof(char *) * (list_size(tmp) + 1));
    while (tmp)
    {
        str = ft_strjoin("=", tmp->value);
        envp[i] = ft_strjoin(tmp->key, str);
        i++;
        free(str);
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return (envp);
}
void check_command(t_command *command, t_env **env_vars)
{
    if (ex_strcmp("cd", command->cmd[0]) == 0)
        cd(command, *env_vars);
    else if (ex_strcmp("echo", command->cmd[0]) == 0)
        echo(command);
    else if (ex_strcmp("export", command->cmd[0]) == 0)
        export(command, env_vars);
    else if (ex_strcmp("env", command->cmd[0]) == 0)
        env(*env_vars);
    else if (ex_strcmp("unset", command->cmd[0]) == 0)
        unset(command, env_vars);
    else if (ex_strcmp("exit", command->cmd[0]) == 0)
        exit_shell(command);
    else
       commands(command, envp_array(*env_vars));
}

