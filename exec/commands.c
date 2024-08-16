#include "minishell_exec.h"

int execute_command(t_command *command, char *path)
{
    // pid_t pid;

    // pid = fork();
    // if (pid == -1)
    // {
    //     perror("fork");
    //     return (1);
    // }
    // else if (pid == 0)
    // {
    //     // child process
    if (execve(path, command->cmd, NULL) == -1)// ls >> a.txt
        perror("execve"); 
    // }
    // else {
    //     // Parent process
    //     waitpid(pid, NULL, 0); // Wait for the child process to finish
    // }
    return (0);
}

