/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:46:59 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/03 13:56:06 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_strs(char **strs)
{
	int	i;

	i = 0;
	if (!strs || strs[i] == 0)
		return ;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

void	free_cmds(t_command **cmds)
{
	t_command	*p;
	t_command	*tmp;

	if (!cmds || !*cmds)
		return ;
	tmp = *cmds;
	while (tmp != NULL)
	{
		p = tmp->next;
		free_strs(tmp->cmd);
		free_strs(tmp->redirection);
		free(tmp);
		tmp = p;
	}
	// *cmds = NULL;
}
