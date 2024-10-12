#include "../includes/minishell_exec.h"
static void export_display(t_env **env_vars)
{
    t_env *env_copy;

    env_copy = env_vars_copy(env_vars); // made a copy
    sort_vars(&env_copy); // sorted the copy
    while (env_copy) // printing the sorted copy
    {
        if (ex_strcmp(env_copy->key, "_") == 0)
        {
            env_copy = env_copy->next;
            continue;
        }
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
        return (1);
    i++;
    while (key[i])
    {
        if (!ft_isalnum(key[i]) && key[i] != '_')
            return (1);
        i++;
    }
    return (0);
}

void update_key(char *cmd, t_env **env)
{
    if (ft_strcmp(get_str(cmd, "key"), "_") == 0) // if key is _
        update_var(*env, get_str(cmd, "key"), // handle _ case
                ft_strdup("_"));
    else
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
            update_var(*env, get_str(cmd, "key"), // handle _ case
                get_str(cmd, "value"));
    } 
    }
   
}
int export(t_command *cmds, t_env **env)
{
    int i;
    int exit_status;

    i = 1;
    exit_status = 0;
    if (cmds->cmd[i])
    {
        while (cmds->cmd[i])
        {
            if (export_syntax(get_str(cmds->cmd[i], "key"))) // Is Valid identifier
            {
                print_error("export", cmds->cmd[i], "not a valid identifier");
                exit_status = 1;
            }
            else if (get_env_key(*env, get_str(cmds->cmd[i], "key"))) // if key alrdy existing
                update_key(cmds->cmd[i], env);
            else 
            {
                add_env_back(env, new_variable(get_str(cmds->cmd[i], "key"), 
                    get_str(cmds->cmd[i], "value")));
            }
            i++;
        }
    }
    else
        export_display(env);
    return (exit_status);
}
int  unset(t_command *cmds, t_env **env_vars)
{
    int i;
    char *key;
    int exit_status;
    i = 1;
    exit_status = 0;
    while (cmds->cmd[i])
    {
        key = ft_strdup(cmds->cmd[i]);
        if (export_syntax(key))
        {
            print_error("unset", cmds->cmd[i], ":not a valid identifier");
            exit_status = 1;
        }
        else 
        {
            if (ex_strcmp(key, "_") != 0)
                delete_env(env_vars, cmds->cmd[i]);
        }
        i++;
    }
    return (exit_status);
}
