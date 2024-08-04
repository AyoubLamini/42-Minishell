/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 08:47:16 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/03 07:39:21 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// int	count_tokens(char **args, char token)
// {
// 	int	i;
// 	int	nb;

// 	nb = 0;
// 	i = 0;
// 	while (args[i])
// 	{
// 		if (args[i][0] == token)
// 			nb++;
// 		i++;
// 	}
// 	return (nb);
// }

int	ft_strslen(char **map)
{
	int	i;

	i = 0;
	while (map[i])
		i++;
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0')
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

t_command	*allocate_node(char **args, int start, int end)
{
	t_command	*node;

	node = (t_command *)malloc(sizeof(t_command));
	if (!node)
		return (NULL);
	while (start < end && ft_strcmp(args[start], ">>") && ft_strcmp(args[start], "<<") 
		&& ft_strcmp(args[start], ">") && ft_strcmp(args[start], "<"))
		start++;
	node->cmd = (char **)malloc(sizeof(char *) * (start + 1));
	if (!node->cmd)
		return (NULL);
	node->redirection = (char **)malloc(sizeof(char *) * (end - start) + 1);
	if (!node->redirection)
		return (NULL);
	node->next = NULL;
	return (node);
}

t_command	*get_command(char **args, int start, int end)
{
	t_command *node;
	int		ci;
	int		ri;

	ci = 0;
	ri = 0;
	node = allocate_node(args, start, end);
	while (start < end && ft_strcmp(args[start], ">>") && ft_strcmp(args[start], "<<") 
		&& ft_strcmp(args[start], ">") && ft_strcmp(args[start], "<"))
	{
		node->cmd[ci] = args[start];
		start++;
		ci++;
	}
	node->cmd[ci] = NULL;
	while (start < end)
	{
		node->redirection[ri] = args[start];
		start++;
		ri++;
	}
	node->redirection[ri] = NULL;
	return (node);
}

void	printstrs(char **map)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!map)
		return ;
	while (map[i])
	{
		printf("%s\t", map[i]);
		i++;
	}
	puts("\n");
}

t_command	*split_cmds(char **args)
{
	t_command	*input;
	t_command *node;
	int		i;
	int		start;

	input = NULL;
	i = 0;
	start = i;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
		{
			if (start < i)
			{
				node = get_command(args, start, i);
				lstadd_back(&input, node);
			}
			i++;
			start = i;
		}
		else
			i++;
	}
	if (start < i)
	{
		node = get_command(args, start, i);
		lstadd_back(&input, node);
	}
	return (input);
}
