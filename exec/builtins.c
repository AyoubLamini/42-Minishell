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

int cd(t_command *command)
{
    int i;

    i = 0;
    if (!command->cmd[1])
    {
        if (!chdir(getenv("HOME")))
            perror("chdir");
    }
   
    return (0);
}
void env(t_env *env_vars)
{
    while (env_vars)
    {
        printf("%s=", env_vars->key);
        printf("%s\n", env_vars->value);
        env_vars = env_vars->next;
    }
    // printf("_=/usr/bin/env\n");
}
void export(t_command *cmds, t_env **env_vars)
{
    int i;
    t_env *env_copy;
    i = 1;
    if (cmds->cmd[i])
    {
        while (cmds->cmd[i])
        {
            if (get_env_variable(*env_vars, get_str(cmds->cmd[i], "key")))
                update_var(*env_vars, get_str(cmds->cmd[i], "key"), get_str(cmds->cmd[i], "value"));
            else
                add_env_back(env_vars, new_variable(get_str(cmds->cmd[i],
                    "key"), get_str(cmds->cmd[i], "value")));
            i++;
        }
    }
    else
    {
        env_copy = env_vars_copy(env_vars); // made a copy
        sort_vars(&env_copy); // sorted the copy
        while (env_copy) // printing the sorted copy
        {
            printf("declare -x %s=%s\n", env_copy->key, env_copy->value);
            env_copy = env_copy->next;
        }
    }
}

void unset(t_command *cmds, t_env **env_vars)
{
    int i;
    i = 1;
    while (cmds->cmd[i])
    {
        delete_env(env_vars, cmds->cmd[i]);
        i++;
    }
}
// void exit(t_command *command)
// {
    
// }