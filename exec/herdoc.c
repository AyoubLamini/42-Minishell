#include "../includes/minishell.h"

static void    ft_rename(t_herdoc *herdoc)
{
    static int id;

    id++;
    herdoc->file = ft_strjoin("/tmp/herdoc", ft_itoa(id));
}

void    ft_heredoc(t_command *command, t_path *path, char *delimiter)
{
    t_heredoc herdoc; 
    int     fd;
    char    *line; 

    herdoc.delimiter = delimiter;
    herdoc.file = NULL;
    ft_rename(&herdoc);
    fd = open(herdoc->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        print_error("heredoc", NULL, strerror(errno));
        exit_status(1, path);
        if (path->is_forked)
            exit(1);
        else
            return ;
    }
    while (1)
    {
        line = readline("> ");
        if (!line)
            break ;
        if (ft_strcmp(line, herdoc->delimiter) == 0)
        {
            free(line);
            break ;
        }
        write(fd, line, ft_strlen(line));
        write(fd, "\n", 1);
        free(line);
    }
    command->last_file = herdoc->file;
    close(fd);
}