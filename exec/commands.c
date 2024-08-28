#include "minishell_exec.h"

static void execution(char *cmd, char **args, char **envp, t_path *path)
{
    if (execve(cmd, args, envp) == -1)
    {
        printf("fesfsefjjsfesf\n");
        print_error(cmd, NULL, strerror(errno));
        exit_status(1, path);
        exit(1);
    }
}

static void absolute_path(t_command *command, char **envp, t_path *path)
{
    struct stat statbuf;
    char *cmd;

    cmd = command->cmd[0];
    if (access(cmd, F_OK) != 0)
    {
        print_error(cmd, NULL, "No such file or directory");
        exit_status(127, path);
        exit(path->exit_status);
    }
    stat(cmd, &statbuf);
    if (S_ISDIR(statbuf.st_mode))
    {
        print_error(cmd, NULL, "is a directory");
        exit_status(126, path); 
        exit(path->exit_status);
    }
    execution(cmd, command->cmd, envp, path);
}

static void commands(t_command *command, t_env **env, char **envp, t_path *path) 
{
    char **ptr;
    int i;
    i = 0;

    if (command->cmd[0] == NULL || ex_strlen(command->cmd[0]) == 0)
    {
        print_error(command->cmd[0], command->cmd[0], "command not found");
        exit_status(127, path);
        exit(path->exit_status);
    }
    if (occur_alpha(command->cmd[0], '/') || command->cmd[0][0] == '.') // absolute path
        absolute_path(command, envp, path);
    if (!get_env_key(*env, "PATH"))
    {
        print_error(command->cmd[0], NULL, "No such file or directory");
        exit_status(127, path);
        exit(127);
    }
    ptr = ex_split(get_env_value(*env, "PATH"), ':'); // get the path
    while (ptr[i])
    {
        ptr[i] = ex_strjoin(ptr[i], "/");
        ptr[i] = ex_strjoin(ptr[i], command->cmd[0]);
        if (access(ptr[i], X_OK) == 0)
            execution(ptr[i], command->cmd, envp, path);
        i++;
    }
    print_error(command->cmd[0], NULL, "command not found"); // if command not found
    exit_status(127, path);
    exit(path->exit_status);
}

void check_command(t_command *command, t_env **env_vars, t_path *path)
{
    if (ex_strcmp("cd", command->cmd[0]) == 0)
        exit_status(cd(command, *env_vars), path); // exit( ,path)
    else if (ex_strcmp("echo", command->cmd[0]) == 0)
        exit_status(echo(command), path);
    else if (ex_strcmp("export", command->cmd[0]) == 0)
        exit_status(export(command, env_vars), path);
    else if (ex_strcmp("env", command->cmd[0]) == 0)
        exit_status(env(*env_vars), path);
    else if (ex_strcmp("unset", command->cmd[0]) == 0)
        exit_status(unset(command, env_vars), path);
    else if (ex_strcmp("exit", command->cmd[0]) == 0)
        exit_shell(command, path);
    else
       commands(command, env_vars, envp_array(*env_vars), path);
}

