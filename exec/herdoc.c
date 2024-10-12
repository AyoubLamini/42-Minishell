#include "../includes/minishell_exec.h"

static char *ft_rename(void)
{
    static int id;
    char *file;

    id++;
    file = ex_strjoin(ft_strdup("./herdoc"), ft_itoa(id));
    return (file);
}

static int ft_heredoc(t_command *command, t_path *path, char *delimiter, t_env **envs)
{
    setup_signals(path, HERDOC_SIG);
    t_heredoc *heredoc;
    int     fd;
    char    *line;
    int    will_expanded;
    char *buffer;
    
    will_expanded = check_will_expanded(delimiter);
    delimiter = get_right_delimeter(delimiter);
    heredoc = lst_heredoc_new(ft_strdup(delimiter), ft_rename());
    lst_heredoc_add_back(&path->heredoc, heredoc);
    buffer = NULL;
    if (path->fd_in == 0)
        path->fd_in = dup(STDIN_FILENO); // save the current stdin
    while (1)
    {
        line = readline("> ");
        if (!line)
        {
            if (g_last_signal == SIGINT)
            {
                g_last_signal = 0;
                exit_status(1, path);
                return (1);
            }
            break ;
        }
        if (ft_strcmp(line, heredoc->delimiter) == 0)
        {
            free(line);
            break ;
        }
        if (will_expanded)
        line = expanding_cmd_herdoc(*envs, line, *path); // i need the envs to expand the line
        buffer = ex_strjoin(buffer, line);
        buffer = ex_strjoin(buffer, "\n");
        free(line);
    }
    fd = open(heredoc->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        print_error("heredoc", NULL, strerror(errno));
        exit_status(1, path);
        exit(path->exit_status);
    }
    write(fd, buffer, ex_strlen(buffer));
    command->last_file = heredoc->file;
    free(buffer);
    close(fd);
    return (0);
}

int is_heredoc(char **red)
{
    int i;

    i = 0;
    if (!red || !red[0])
        return (0);
    while (red[i])
    {
        if (ex_strcmp(red[i], "<<") == 0)
            return (1);
        i++;
    }
    return (0);
}
int handle_herdoc(t_command *command, t_path *path, t_env **envs)
{
    int i;
    i = 0;
    if (!is_heredoc(command->redirection)) // if there is no heredoc
        return (0);
    while (command->redirection[i])
    {
        if (ex_strcmp(command->redirection[i], "<<") == 0)
            if (ft_heredoc(command, path, command->redirection[i + 1], envs))
                return (1);
        i++;
    }
    return (0);
}


