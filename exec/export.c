#include "minishell_exec.h"
static void export_display(t_env **env_vars)
{
    t_env *env_copy;

    env_copy = env_vars_copy(env_vars); // made a copy
    sort_vars(&env_copy); // sorted the copy
    while (env_copy) // printing the sorted copy
    {
        printf("declare -x %s=%s\n", env_copy->key, env_copy->value);
        env_copy = env_copy->next;
    }
}

// static int export_syntax(char *key)
// {

// }

void export(t_command *cmds, t_env **env)
{
    int i;
    
    i = 1;
    if (cmds->cmd[i])
    {
        while (cmds->cmd[i])
        {
            if (get_env(*env, get_str(cmds->cmd[i], "key"))) // if key alrdy existing
            {
                if (ft_strstr(cmds->cmd[i], "+="))
                {
                    printf("Append mode:\n");
                    update_var(*env, get_str(cmds->cmd[i], "key"), 
                        ft_strjoin(get_env(*env, get_str(cmds->cmd[i], "key")),
                            get_str(cmds->cmd[i], "value")));
                } // if append mode
                else
                {
                    update_var(*env, get_str(cmds->cmd[i], "key"), 
                        get_str(cmds->cmd[i], "value"));
                } 
            }
            else // new variable mode
            {
                printf("New var mode:\n");
                add_env_back(env, new_variable(get_str(cmds->cmd[i], "key"), 
                    get_str(cmds->cmd[i], "value")));
            } 
            i++;
        }
    }
    else
        export_display(env);
}