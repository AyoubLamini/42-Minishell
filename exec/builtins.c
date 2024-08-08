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
    if (chdir(command->cmd[1]) != 0)
    {
        return (perror("chdir"), 1);
    }
    printf("succes\n");
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
    i = 1;
    while (cmds->cmd[i])
    {
        add_env_back(env_vars, new_variable(cmds->cmd[i]));
        i++;
    }
     t_env *temp = *env_vars;
    while (temp) {
        printf("%s=%s\n", temp->key, temp->value);
        temp = temp->next;
    }
}