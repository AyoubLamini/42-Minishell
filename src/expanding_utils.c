/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/11 11:51:02 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/09/13 11:06:42 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../exec/minishell_exec.h"

void	syntax_error_messages(int code)
{
	if (code == -1)
		write(1, "Minishell: syntax error near unexpected token `newline'\n", 57);
	else if (code == -2)
		write(1, "Minishell: syntax error near unexpected token `>'\n", 51);
	else if (code == -3)
		write(1, "Minishell: syntax error near unexpected token `<'\n", 51);
	else if (code == -4)
		write(1, "bash: syntax error near unexpected token `|'\n", 46); 
}

int	my_strcmp(char *s1, char *s2)
{
	int	i;

	if (!s1 || !s2)
		return (-1);
	i = 0;
	while (s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0' && s2[i] != '\'' && s2[i] != '"')
		i++;
	return (s1[i] - s2[i]);
}

char	*get_env_variable(t_env *env, char *env_key)
{
	t_env	*tmp;
	tmp = env;
	while (tmp)
	{
		if (my_strcmp(tmp->key, env_key) == 0)
			return (tmp->value);
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
		new[i]= s1[i];
		i++;
	}
	new[i++] = s2;
	new[i] = 0;
	return (new); 
}

char	**join_two_double_strs(char **s1, char **s2)
{
	char	**new;
	int		len1;
	int		len2;
	int		i;
	int		j;
	
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
		new[i]= s1[i];
		i++;
	}
	while (s2[j])
		new[i++] = s2[j++];
	new[i] = 0;
	return (new); 
}