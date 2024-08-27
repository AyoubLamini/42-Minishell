/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 11:51:02 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/25 15:59:19 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../exec/minishell_exec.h"

int	my_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && s2[i] != '\'' && s2[i] != '"')
	{
		i++;
	}
	return (s1[i] - s2[i]);
}

char	*get_env_variable(t_env *env, char *env_key)
{
	t_env	*tmp;
	tmp = env;
	while (tmp)
	{
		if (my_strcmp(tmp->key, env_key) == 0)
		{
			return (tmp->value);
		}
		tmp = tmp->next;
	}
	return (NULL);
}

static char *get_key(char *str) // Hello=Friend  a+=  a     a=
{
	char	*key;
	int 	i;

	i = 0;
	key = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!key)
		return (NULL);
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		key[i] = str[i];
		i++;
	}
	key[i] = '\0';
	return (key);
}

static char *get_value(char *str)  // OLDPWD   | OLDPWD=  | OLDPWD=hello
{
	char	*value;
	int 	i;
	int 	j;

	i = 0;
	j = 0;
	value = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!value)
		return (NULL);
	while (str[j] && str[j] != '=') // skip the key
			j++;
	if (str[j] != '=') // if no seperator after key
		return (free(value), NULL);
	j++; // skip '=' 
	while (str[j]) // fill the value
	{
		value[i] = str[j]; 
		i++;
		j++;
	}
	value[i] = '\0';
	return (value);
}

static char *get_sep(char *str)
{
	char	*sep;
	int 	i;
	int 	j;

	i = 0;
	j = 0;
	sep = (char *)malloc(sizeof(char) * ft_strlen(str) + 1);
	if (!sep)
		return (NULL);
	while (str[j] && str[j] != '=' && str[j] != '+')
			j++;
	if (str[j] == '=')
	{
		sep[i++] = '=';
		sep[i++] = '\0';
	}
	else if (str[j] == '+' && str[j+1] == '=')
	{
		sep[i++] = '+';
		sep[i++] = '=';
		sep[i] = '\0';
	}
	else
		return (NULL);
	return (sep);
}

char *get_str(char *var, char *type) // allocate memory and returns key or value or sep
{ 
	if (!var || !type)
		return (NULL);
	if (ex_strcmp(type, "key") == 0)
		return (get_key(var));
	else if (ex_strcmp(type, "value") == 0)
		return (get_value(var));
	else if (ex_strcmp(type, "sep") == 0)
		return (get_sep(var));
	return (NULL);
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