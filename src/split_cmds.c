/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 08:47:16 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/13 04:50:12 by ybouyzem         ###   ########.fr       */
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

t_command	*allocate_node()
{
	t_command	*node;
	
	node = (t_command *)malloc(sizeof(t_command));
	if (!node)
		return (printf("malloc failed!\n"), exit(1), NULL);
	node->cmd = NULL;
	node->redirection = NULL;
	node->is_ambiguous = -1;
	node->ambiguous_file = NULL;
	node->next = NULL;
	node->last_file = NULL;

	return (node);
}

int	ft_is_red(char *str)
{
	if (!ft_strcmp(str, ">") || !ft_strcmp(str, ">>") || !ft_strcmp(str, "<"))
		return (1);
	return (0);
}

t_command	*get_command(char **args, t_env *envs, int start, int end, t_path *path)
{
	t_command	*node;
	static char		**tmp;
	static int 		i;

	tmp = NULL;
	node = allocate_node();
	while (start < end && args[start])
	{
		if (ft_is_red(args[start]))
		{
			node->redirection = join_double_strs_with_str(node->redirection, args[start]);
			start++;
			tmp = expanding_red(node, envs, args[start], path, start);
			node->redirection = join_two_double_strs(node->redirection, tmp);
			start++;
		}
		else if (!ft_strcmp(args[start], "<<"))
		{
			node->redirection = join_double_strs_with_str(node->redirection, args[start]);
			start++;
			node->redirection = join_double_strs_with_str(node->redirection, args[start]);
			start++;
		}
		else
		{
			tmp = expanding_cmd(envs, args[start], path, i);
			node->cmd = join_two_double_strs(node->cmd, tmp);
			start++;
		}
	}
	if (node->redirection == NULL)
	{
		node->redirection = (char **)malloc(sizeof(char *));
		node->redirection[0] = 0;
	}
	if (node->cmd == NULL)
	{
		node->cmd = (char **)malloc(sizeof(char *));
		node->cmd[0] = 0;
	}
	i++;
	if (!args[end])
		i = 0;
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

t_command	*split_cmds(char **args, t_env *envs, t_path *path)
{
	t_command	*input;
	t_command *node;
	int		i;
	int		start;

	node = NULL;
	input = NULL;
	i = 0;
	start = i;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0 || !args[i])
		{
			if (start < i)
			{
				node = get_command(args, envs, start, i, path);
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
		node = get_command(args, envs, start, i, path);
		lstadd_back(&input, node);
	}
	return (input);
}
