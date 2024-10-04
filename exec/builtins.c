#include "minishell_exec.h"

static int is_valid_option(char *str)
{
    int i;
  
    i = 0;
    if (str[i] == '-' && str[i+1] == 'n')
    {
        i++;
        while (str[i])
        {
            if (str[i] != 'n' && str[i])
                return (1);
            i++;
        }   
    }
    else
        return (1);
    return (0);
}

int echo(t_command *command)
{
    int is_valid;
    int new_line;
    int i;

    i = 1;
    new_line = 1;
    is_valid = 0;
    while (command->cmd[i] && is_valid == 0)
    {
        is_valid = is_valid_option(command->cmd[i]);
        if (is_valid == 0)
        {
            new_line = 0;
            i++;
        }
        else
            break;
    }
    while (command->cmd[i])
    {
        printf("%s", command->cmd[i]);
        if (command->cmd[i + 1])
            printf(" ");
        i++;
    }
    if (new_line == 1)
        printf("\n");
    return (0);
}

int env(t_env *env_vars)
{
    while (env_vars)
    {
        if (get_env_value(env_vars, env_vars->key))
        {
            printf("%s=", env_vars->key);
            printf("%s\n", env_vars->value);
        }
        env_vars = env_vars->next;
    }
    return (0);
    // printf("_=/usr/bin/env\n");
}

int pwd() // pwd
{
    char *cwd;
    cwd = getcwd(NULL, 0);
    printf("%s\n", cwd);
    free(cwd);
    return (0);
}

void exit_shell(t_command *command, t_path *path)
{
    if (!command->cmd[1])
    {
        printf("exit\n");
        exit(path->exit_status); // the exit status will be last command exit status
    }
    if (!ft_is_numeric(command->cmd[1])) // bash: exit: dwaf468: numeric argument required
    {
        ft_putstr_fd("exit\n", 1);
        print_error("exit", command->cmd[1], "numeric argument required");
        exit(255);
    }
    else if (command->cmd[2])
    {
        ft_putstr_fd("exit\n", 1);
        print_error("exit", NULL, "too many arguments");
        exit_status(1, path);
    }
    else
    {
        // printf("exit");
        exit(ft_atoi(command->cmd[1]));
    }
}