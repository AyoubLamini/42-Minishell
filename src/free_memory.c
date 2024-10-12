/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_memory.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 12:46:59 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/12 07:40:12 by ybouyzem         ###   ########.fr       */
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
		free_str(strs[i]);
		i++;
	}
	free(strs);
	strs = NULL;
}

void	free_envs(t_env *envs)
{
	t_env	*current;
	t_env	*tmp;

	current = envs;
	while (current)
	{
		tmp = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = tmp;
	}
}

void	free_cmds(t_command *cmds)
{
	t_command	*current;
	t_command	*tmp;

	if (!cmds)
		return ;
	current = cmds;
	while (current)
	{
		tmp = current->next;
		free_strs(current->cmd);
		free_strs(current->redirection);
		free(current);
		current->cmd = NULL;
		current->redirection = NULL;
		current = tmp;
	}
	cmds = NULL;
}

void	free_str(char *str)
{
	if (str)
		free(str);
	str = NULL;
}
