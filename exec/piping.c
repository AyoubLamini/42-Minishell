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
static int child_process(t_command *command, t_env **env, int *fd, int *input_fd)
{
    if (*input_fd != 0) // If there is input from a previous pipe
    {
        if (dup2(*input_fd, STDIN_FILENO) < 0) // Redirect stdin to the previous pipe's read end
        {
            perror("dup2");
            return (1);
        }
            close(*input_fd);
    }
    if (command->next) // If there is a next command, redirect stdout to the current pipe's write end
    {
        if (dup2(fd[1], STDOUT_FILENO) < 0)
        {
                perror("dup2");
                return (1);
        }
    }
    close(fd[0]); // Close the unused pipe ends
    close(fd[1]);
    check_command(command, env); // Execute the command
    return (0);
}

static int parent_process(t_command *command, int *fd, int *input_fd)
{
    if (*input_fd != 0)
        close(*input_fd); // Close the previous pipe's read end

    if (command->next) // If there is a next command
    {
        close(fd[1]); // Close the current pipe's write end
        *input_fd = fd[0]; // Save the read end for the next command
    }
    else
    {
            close(fd[0]); // Close the last pipe's read end
            *input_fd = 0;
    }
    return (0);
}
void piping(t_command *command, t_env **env_vars, int *id)
{
    (void)id;
    int pid;
    int fd[2];
    static int input_fd = 0; // To store input from the previous command's pipe

    if (command->next)  // Create a pipe if there is a next command
        if (create_pipe(fd))
            return ;
    pid = fork();
    if (pid < 0)
        return (perror("fork"));
    if (pid == 0)
    {
        if (child_process(command, env_vars, fd, &input_fd))
            return ;
    }
    else
    {
        if (parent_process(command, fd, &input_fd))
            return ;
    }
    waitpid(pid, NULL, 0);
       
    return ;
}
