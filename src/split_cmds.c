/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 08:47:16 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/09/05 12:12:32 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*concat_strs(char* s1, char* s2)
{
    int		len1;
    int		len2;
    int		i;
    int		j;
    char	*result; 
	
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	i = 0;
	j = 0;
	result = (char*)malloc((len1 + len2 + 1) * sizeof(char));
    if (!result)
		return (NULL);
    while (s1[i] != '\0') {
        result[i] = s1[i];
        i++;
    }
    while (s2[j] != '\0') {
        result[i + j] = s2[j];
        j++;
    }
    result[i + j] = '\0';
    return (result);
}

int	ft_strslen(char **map)
{
	int	i;

	if (map == NULL)
		return (0);
	i = 0;
	while (map[i])
		i++;
	return (i);
}

int	ft_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (-1);
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

t_command	*get_command(char **args, t_env *envs, int start, int end)
{
	t_command	*node;
	int			ci;
	int			ri;
	char		**tmp;
	int			j;

	ci = 0;
	ri = 0;
	node = allocate_node(args, start, end);
	while (start < end && ft_strcmp(args[start], ">>") && ft_strcmp(args[start], "<<") 
		&& ft_strcmp(args[start], ">") && ft_strcmp(args[start], "<"))
	{
		j = 0;
		tmp = expanding_cmd(envs, args[start]);
		if (tmp)
		{
			while (tmp[j])
				node->cmd[ci++] = tmp[j++];
		}
		start++;
	}
	node->cmd[ci] = NULL;
	while (start < end)
	{
		node->redirection[ri] = expanding_red(envs, args[start]);
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
		printf("|%s|\n", map[i]);
		i++;
	}
}

t_command	*split_cmds(char **args, t_env *envs)
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
				node = get_command(args, envs, start, i);
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
		node = get_command(args, envs, start, i);
		lstadd_back(&input, node);
	}
	//print_list(input);
	return (input);
}
