/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:52:45 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/13 01:54:06 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"

char	**join_two_double_strs(char **s1, char **s2)
{
	char	**new;
	int		len1;
	int		len2;
	int		i;
	int		j;
	
	new = NULL;
	i = 0;
	j = 0;
	if (!s2)
		return (s1);
	if (!s1)
		return (s2);
	if (!s1 && !s2)
		return (NULL);
	len1 = ft_strslen(s1);
	len2 = ft_strslen(s2);
	new = (char **)malloc(sizeof(char *) * (len1 + len2 + 1));
	if (!new)
		return (NULL);
	while (s1[i])
	{
		new[i]= ft_strdup(s1[i]);
		i++;
	}
	while (s2[j])
		new[i++] = ft_strdup(s2[j++]);
	new[i] = 0;
	return (free_strs(s1), free_strs(s2), new); 
}

char	**join_double_strs_with_str(char **s1, char *s2)
{
	char	**new;
	int		len;
	int		i;

	i = 0;
	if (!s2)
		return (s1);
	if (!s1 || !*s1)
	{
		new = (char **)malloc(sizeof(char *) * 2);
		if (!new)
			return (NULL);
		new[0] = s2;
		new[1] = NULL;
		return (new);
	}
	len = ft_strslen(s1);
	new = (char **)malloc(sizeof(char *) * (len + 2));
	if (!new)
		return (NULL);
	while (s1[i])
	{
		new[i]= ft_strdup(s1[i]);
		i++;
	}
	new[i++] = ft_strdup(s2);
	new[i] = 0;
	return (free_strs(s1), free_str(s2), new);
}

void	print_envs(t_env *envs)
{
	t_env *p;	
	p = envs;

	while (p != NULL)
	{
		printf("%s", p->key);
		printf("=%s\n", p->value);
		p = p->next;
	}
}

void delete_env(t_env **env, char *env_key)
{
	t_env *temp;
	t_env *del;
	t_env *prev;
	temp = *env;
	if (!env || !env_key)
        return ;
	prev = NULL;
	while (temp)
	{
		if (ex_strcmp(temp->key, env_key) == 0)
		{
			del = temp;
			if (prev)
				prev->next = temp->next;
			else
				*env = temp->next;
			free(del->key);
			free(del->value);
			free(del);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
	return ;
}

void	add_env_back(t_env **envs, t_env *new)
{
	t_env	*p;

	p = *envs;
	if (*envs == NULL)
		*envs = new;
	else
	{
		while (p->next != NULL)
			p = p->next;
		p->next = new;
	}
}