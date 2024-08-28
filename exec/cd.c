#include "minishell_exec.h"


int exec_cd(t_env *env, char *var, int check)
{
    char *old_wd;
    char *cwd;
    char *path;

    path = NULL;
    old_wd = (char *)malloc(sizeof(char) * PATH_MAX);
    cwd = (char *)malloc(sizeof(char) * PATH_MAX);
    getcwd(old_wd, PATH_MAX);
    if (!check) // if check == 0 | path is a env variable 
        path = get_env_value(env, var);
    else // if path passed by user
        path = var;
    if (!path)
        return (print_error("cd", var, "not set"), 1); // if path not set
    if (chdir(path) == -1)
        return (print_error("cd", path, strerror(errno)), errno);
    else
    {
        if (get_env_key(env, "OLDPWD"))
            update_var(env, "OLDPWD", old_wd);
        else
            add_env_back(&env, new_variable(ft_strdup("OLDPWD"), old_wd));
        getcwd(cwd, PATH_MAX);
        update_var(env, "PWD", cwd);
    }
    return (0);
}

int cd(t_command *command, t_env *env) // need to updated PWD and OLD_PWD
{
    if (!command->cmd[1] || ex_strcmp(command->cmd[1], "~") == 0)
    {
        if (exec_cd(env, "HOME", 0))
            return (1);
    }
    else if (ex_strcmp(command->cmd[1], "-") == 0)
    {
        if (exec_cd(env, "OLDPWD", 0))
            return (1);
        else
            printf("%s\n", get_env_value(env, "PWD"));
    }
    else
    {
        if (exec_cd(env, command->cmd[1], 1))
            return (1);
    }
    return (0);
}
