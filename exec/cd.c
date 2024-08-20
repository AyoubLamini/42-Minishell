#include "minishell_exec.h"


int exec_cd(t_env *env, char *var)
{
    char buffer[PATH_MAX];
    char *cwd;
    cwd = getcwd(buffer, PATH_MAX);
    char *path;
    if (env)
        path = get_env_value(env, var);
    else
        path = var;
    if (chdir(path) == -1)
    {
        print_error("cd", path, strerror(errno));
        return (1);
    }
    else
    {
        printf("success\n");
        update_var(env, "OLDPWD", cwd);
        cwd = getcwd(buffer, PATH_MAX);
        update_var(env, "PWD", cwd);
        return (0);
    }
    return (0);
}

int cd(t_command *command, t_env *env) // need to updated PWD and OLD_PWD
{
   
    if (!command->cmd[1] || ex_strcmp(command->cmd[1], "~") == 0)
    {

        if (exec_cd(env, "HOME"))
            return (1);
    }
    else if (ex_strcmp(command->cmd[1], "-") == 0)
    {
        if (exec_cd(env, "OLDPWD"))
            return (1);
    }
    else
    {

        if (exec_cd(NULL, command->cmd[1]))
            return (1);
    }
    return (0);
}