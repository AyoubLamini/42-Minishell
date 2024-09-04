#include "minishell_exec.h"

static int std_out(t_command *command, int pos, t_exec *file_d)
{
    int fd;
    if (file_d->out == 1)  
        file_d->out = dup(STDOUT_FILENO); // when 
    fd = open(command->redirection[pos], O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
        return (perror("open"), 1);
    if (dup2(fd, STDOUT_FILENO) == -1)
    {  
        perror("dup2");
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}
static int std_out_append(t_command *command, int pos, t_exec *file_d)
{
    int fd;

    if (file_d->out == 1)  
        file_d->out = dup(STDOUT_FILENO);
    fd = open(command->redirection[pos], O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1)
        return (perror("open"), 1);
    if (dup2(fd, STDOUT_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}
static int std_in(t_command *command, int pos, t_exec *file_d)
{
    int fd;

    if (file_d->in == 0)  
        file_d->in = dup(STDIN_FILENO); // only in beggininng we dup std input to 0 || because if its 3 for 
    fd = open(command->redirection[pos], O_RDONLY);
    if (fd == -1)
        return (perror("open"), 1);
    if (dup2(fd, STDIN_FILENO) == -1)
    {
        perror("dup2");
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}
// static int here_doc(t_command *command, int pos)
// {

// }


int handle_redirection(t_command *command, t_exec *file_d)
{
    int i;
    i = 0;
    while (command->redirection[i])
    {
        if (ex_strcmp(command->redirection[i], ">") == 0)
            std_out(command, i + 1, file_d);
        else if (ex_strcmp(command->redirection[i], ">>") == 0)
            std_out_append(command, i + 1, file_d);
        else if (ex_strcmp(command->redirection[i], "<") == 0)
            std_in(command, i + 1, file_d);
        // else if (ft_strcmp(command->redirection[i], "<<") == 0)
        //     here_doc(command, i + 1);
        i++;
    }
    return (0);
}
