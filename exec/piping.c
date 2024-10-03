#include "minishell_exec.h"

static int create_pipe(int *fd)
{
    if (pipe(fd) < 0)
    {
        perror("pipe");
        return (1);
    }
    return (0);
}
static int child_process(t_command *command, t_env **env, int *fd, int *input_fd, t_path *path)
{
    setup_signals(path, UNSET_SIG);
    if (*input_fd != -1) // If there is previous pipe
    {
        if (dup2(*input_fd, STDIN_FILENO) < 0) // read from stored previous pipe fd
        {
            perror("dup2");
            return (1);
        }
        close(*input_fd); // close it, cause we longer need it
    }
    if (command->next) // iF Next Command: pipe[1] = STDOUT;
    {
       
        if (dup2(fd[1], STDOUT_FILENO) < 0)
            return (perror("dup2"), 1);
        close(fd[0]);
        close(fd[1]);
    }
    if (command->redirection[0])
    {
        handle_redirection(command, path);
    }
    check_command(command, env, path);
    exit(0);
}

static int parent_process(t_command *command, int *fd, int *input_fd)
{
    if (*input_fd != -1)
        close(*input_fd); // Close the previous pipe's read end

    if (command->next) // If there is a next command
    {
        close(fd[1]); // Close the current pipe's write end
        *input_fd = fd[0]; // Save the read end for the next command
    }
    else
    {
            close(fd[0]); // Close the last pipe's read end
    }
    return (0);
}
void piping(t_command *command, t_env **env_vars, int *input_fd, t_path *path)
{
    int pid;
    int fd[2];
    if (command->next)  // iF Next Command = Create Pipe
        if (create_pipe(fd))
            return ;
    pid = fork(); // Fork Child Process
    path->is_forked = 1;
    if (pid < 0)
        return (perror("fork"));
    if (pid == 0) // Child process
    {
        if (child_process(command, env_vars, fd, input_fd, path))
            return ;
    }
    else
    {
        if (parent_process(command, fd, input_fd))
            return ;
    }
    return ; 
}
