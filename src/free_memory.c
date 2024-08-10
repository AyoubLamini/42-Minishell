/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:46:59 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/10 11:28:51 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	if (!strs)
		return ;
	
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}
void	free_args(char **strs)
{
	int	i;

	i = 0;
	if (!strs)
		return ;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	free_cmds(t_command *cmds)
{
	t_command	*current;
	t_command	*tmp;

	current = cmds;
	while (current)
	{
		tmp = current->next;
		free_strs(current->cmd);
		free_strs(current->redirection);
		free(current);
		current = tmp;
	}
}
