#include "../includes/minishell_exec.h"

t_heredoc *lst_heredoc_new(char *delimiter, char *file)
{
    t_heredoc *new;

    new = (t_heredoc *)malloc(sizeof(t_heredoc));
    if (!new)
        return (NULL);
    new->delimiter = delimiter;
    new->file = file;
    new->buffer = NULL;
    new->will_expand = 0;
    new->next = NULL;
    return (new);
}

void    lst_heredoc_add_back(t_heredoc **lst, t_heredoc *new)
{
    t_heredoc   *p;

    p = *lst;
    if (*lst == NULL)
        *lst = new;
    else
    {
        while (p->next)
            p = p->next;
        p->next = new;
    }
}
void process_heredocs(t_path *path)
{
    t_heredoc *current = path->heredoc;
    while (current)
    {
        printf("Heredoc file: %s\n", current->file);
        printf("Heredoc delimiter: %s\n", current->delimiter);
        current = current->next;
    }
}
void clear_herdocs(t_path *path)
{
    t_heredoc *current = path->heredoc;
    t_heredoc *tmp;
    while (current)
    {
        tmp = current;
        current = current->next;
        unlink(tmp->file);
        free(tmp->file);
        free(tmp);
    }
    path->heredoc = NULL;
}

