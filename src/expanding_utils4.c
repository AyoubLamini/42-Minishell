/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils4.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 01:52:45 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/13 04:46:57 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"

char	**join_two_double_strs(char **s1, char **s2)
{
	t_vars	vars;

	vars = ft_initialize_vars();
	if (!s2)
		return (s1);
	if (!s1)
		return (s2);
	if (!s1 && !s2)
		return (NULL);
	vars.len = ft_strslen(s1) + ft_strslen(s2);
	vars.res = ft_allocate(vars.len);
	while (s1[vars.i])
	{
		vars.res[vars.i] = ft_strdup(s1[vars.i]);
		vars.i++;
	}
	while (s2[vars.j])
		vars.res[vars.i++] = ft_strdup(s2[vars.j++]);
	vars.res[vars.i] = 0;
	return (free_strs(s1), free_strs(s2), vars.res);
}

char	**join_double_strs_with_str(char **s1, char *s2)
{
	t_vars	vars;

	vars = ft_initialize_vars();
	if (!s2)
		return (s1);
	if (!s1 || !*s1)
	{
		vars.res = (char **)malloc(sizeof(char *) * 2);
		if (!vars.res)
			return (NULL);
		vars.res[0] = s2;
		vars.res[1] = NULL;
		return (vars.res);
	}
	vars.res = (char **)malloc(sizeof(char *) * (ft_strslen(s1) + 2));
	if (!vars.res)
		return (NULL);
	while (s1[vars.i])
	{
		vars.res[vars.i] = ft_strdup(s1[vars.i]);
		vars.i++;
	}
	vars.res[vars.i++] = ft_strdup(s2);
	vars.res[vars.i] = 0;
	return (free_strs(s1), free_str(s2), vars.res);
}

void	print_envs(t_env *envs)
{
	t_env	*p;

	p = envs;
	while (p != NULL)
	{
		printf("%s", p->key);
		printf("=%s\n", p->value);
		p = p->next;
	}
}

void	delete_env(t_env **env, char *env_key)
{
	t_env	*temp;
	t_env	*del;
	t_env	*prev;

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
