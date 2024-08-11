/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 11:51:02 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/11 15:50:09 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../exec/minishell_exec.h"

char	*get_env_variable(t_env *env, char *env_key)
{
	t_env	*tmp;
	
	tmp = env;
	while (tmp)
	{
		// printf("key: %s\n", tmp->key);
		// printf("env_key: %s\n", env_key);
		if (env_key[0] == '$' && ft_strcmp(tmp->key, env_key+1) == 0)
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

char *get_str(char *var, char *type) // allocate memory and returns key or value string
{
	char *str;
	int	i;
	int j;
	
	i = 0;
	j = 0;
	str = (char *)malloc(sizeof(char) * ft_strlen(var)); 
	if (!str)
		return (NULL);
	if (ex_strcmp(type, "key") == 0)
		while (var[i] != '=')
		{
			str[i] = var[i];
			i++;
		}
	else if (ex_strcmp(type, "value") == 0)
	{
		while (var[j] != '=')
			j++;
		j++;
		while (var[j])
			str[i++] = var[j++]; 
	}
	str[i] = '\0';
	return (str);
}


t_env	*new_variable(char *env_key, char *env_value)
{
	t_env	*node;

	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = env_key;
	node->value = env_value;
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