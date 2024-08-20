#include "minishell_exec.h"


int exec_cd(t_env *env, char *var, int check)
{
    char *old_wd;
    char *cwd;
    char *path;

    old_wd = (char *)malloc(sizeof(char) * PATH_MAX);
    cwd = (char *)malloc(sizeof(char) * PATH_MAX);
    getcwd(old_wd, PATH_MAX);
    if (!check)
        path = get_env_value(env, var);
    else
        path = var;
    if (!path)
        return (print_error("cd", var, "not set"), 1); // if path not set
    if (chdir(path) == -1)
    {
        print_error("cd", path, strerror(errno));
        return (1);
    }
    else
    {
        update_var(env, "OLDPWD", old_wd);
        getcwd(cwd, PATH_MAX);
        update_var(env, "PWD", cwd);
        return (0);
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
    }
    else
    {
        if (exec_cd(env, command->cmd[1], 1))
            return (1);
    }
    return (0);
}
