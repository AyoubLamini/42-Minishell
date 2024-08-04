/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:55:50 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/03 08:52:09 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_env_variable(t_env *env, char *variable)
{
	t_env	*tmp;
	
	tmp = env;
	while (tmp)
	{
		if (tmp->key == variable)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}

t_env	*new_variable(char	*var)
{
	t_env	*node;
	int		i;
	int		j;

	i = 0;
	j = 0;
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = (char *)malloc(sizeof(char *) * ft_strlen(var));
	if (!node->key)
		return (NULL);
	node->value = (char *) malloc(sizeof(char) * ft_strlen(var));
	if (!node->value)
		return (NULL);
	while (var[i] != '=')
	{
		node->key[i] = var[i];
		i++;
	}
	node->key[i] = '\0';
	i++;
	while (var[i])
	{
		node->value[j] = var[i];
		i++;
		j++;
	}
	node->value[j] = '\0';
	node->next = NULL;
	return (node);
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

t_env   *expanding(char **env)
{
	int		i;
	t_env	*node;
	t_env	*env_vars;

	env_vars = NULL;
	i = 0;
	while (env[i])
	{
		node = new_variable(env[i]);
		add_env_back(&env_vars, node);
		i++;
	}
	return (env_vars);
}