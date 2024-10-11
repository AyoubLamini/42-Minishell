/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_split.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 11:26:37 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/11 02:18:46 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_count_word(char  *old_cmd)
{
	int	lw;
	int	i;

	lw = 0;
	i = 0;
	while (old_cmd[i])
	{
		if (old_cmd[i] == '"')
		{
			lw++;
			i++;
			while (old_cmd[i] && old_cmd[i] != '"')
				i++;
			i++;
		}
		else if (old_cmd[i] == '\'')
		{
			lw++;
			i++;
			while (old_cmd[i] && old_cmd[i] != '\'')
				i++;
			i++;
		}
		else
		{
			lw++;
			while (old_cmd[i] && old_cmd[i] != '\'' && old_cmd[i] != '"')
				i++;
		}
	}
	return (lw);
}

char    **expanding_split(char  *old_cmd)
{
	char    **res;
	int		i;
	int		index;
	int		start;
	
	index = 0;
	i = 0;
	res = (char **)malloc(sizeof(char *) * (ft_count_word(old_cmd) + 1));
	if (!res)
		return (NULL);
	while (old_cmd[i])
	{
		start = i;
		if (old_cmd[i] == '"')
		{
			i++;
			while (old_cmd[i] && old_cmd[i] != '"')
				i++;
			i++;
		}
		else if (old_cmd[i] == '\'')
		{
			i++;
			while (old_cmd[i] && old_cmd[i] != '\'')
				i++;
			i++;
		}
		else
		{
			while (old_cmd[i] && old_cmd[i] != '\'' && old_cmd[i] != '"')
				i++;
		}
		res[index] = ft_substr(old_cmd, start, i - start);
		index++;
	}
	res[index] = 0;
	return (free_str(old_cmd), res);
}
