/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:32:24 by alamini           #+#    #+#             */
/*   Updated: 2024/10/15 11:08:12 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_exec.h"

t_heredoc	*lst_heredoc_new(char *delimiter, char *file)
{
	t_heredoc	*new;

	new = (t_heredoc *)my_malloc(sizeof(t_heredoc), 1);
	if (!new)
		ex_malloc_error();
	new->delimiter = delimiter;
	new->file = file;
	new->buffer = NULL;
	new->will_expand = 0;
	new->next = NULL;
	return (new);
}

void	lst_heredoc_add_back(t_heredoc **lst, t_heredoc *new)
{
	t_heredoc	*p;

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

int	is_any_heredoc(char **red)
{
	int	i;

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

void	process_heredocs(t_path *path)
{
	t_heredoc	*current;

	current = path->heredoc;
	while (current)
	{
		printf("Heredoc file: %s\n", current->file);
		printf("Heredoc delimiter: %s\n", current->delimiter);
		current = current->next;
	}
}

void	clear_herdocs(t_path *path)
{
	t_heredoc	*current;

	current = path->heredoc;
	while (current)
	{
		unlink(current->file);
		current = current->next;
	}
}
