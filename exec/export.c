#include "minishell_exec.h"
static void export_display(t_env **env_vars)
{
    t_env *env_copy;

    env_copy = env_vars_copy(env_vars); // made a copy
    sort_vars(&env_copy); // sorted the copy
    while (env_copy) // printing the sorted copy
    {
        if (env_copy->value)
            printf("declare -x %s=\"%s\"\n", env_copy->key, env_copy->value);
        else
            printf("declare -x %s\n", env_copy->key);
        env_copy = env_copy->next;
    }
}

static int export_syntax(char *key)
{
    int i;

    i = 0;
    if (!key)
        return (1);
    if (!ft_isalpha(key[0]) && key[0] != '_')
        return (free(key), 1);
    i++;
    while (key[i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return (free(key), 1);
        i++;
    }
    return (free(key), 0);
}

void update_key(char *cmd, t_env **env)
{
    if (ex_strcmp(get_str(cmd, "sep"), "+=") == 0) // Append mode
    {
        // Append mode
        update_var(*env, get_str(cmd, "key"), 
            ft_strjoin(get_env_value(*env, get_str(cmd, "key")),
                get_str(cmd, "value")));
    }
    else 
    {
        // Overwrite mode
        if (get_str(cmd, "value"))
            update_var(*env, get_str(cmd, "key"), 
                get_str(cmd, "value"));
    } 
}
int export(t_command *cmds, t_env **env)
{
    int i;
    
    i = 1;
    if (cmds->cmd[i])
    {
        while (cmds->cmd[i])
        {
            if (export_syntax(get_str(cmds->cmd[i], "key"))) // check key syntax
                print_error("export", cmds->cmd[i], "not a valid identifier");
            else if (get_env_key(*env, get_str(cmds->cmd[i], "key"))) // if key alrdy existing
                update_key(cmds->cmd[i], env);
            else // new variable mode
            {
                //printf("New var mode:\n");
                add_env_back(env, new_variable(get_str(cmds->cmd[i], "key"), 
                    get_str(cmds->cmd[i], "value")));
            }
            i++;
        }
    }
    else
        export_display(env);
    return (0);
}
int  unset(t_command *cmds, t_env **env_vars)
{
    int i;
    i = 1;
    char *key;
    while (cmds->cmd[i])
    {
        key = get_str(cmds->cmd[i], "key");
        if (export_syntax(key))
            print_error("unset", cmds->cmd[i], ":not a valid identifier");
        else
            delete_env(env_vars, cmds->cmd[i]);
        i++;
    }
    return (0);
}
