/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:30:36 by alamini           #+#    #+#             */
/*   Updated: 2024/10/16 16:01:47 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_exec.h"

void	reset_fd(t_path *path)
{
	if (path->fd_in != 0)
	{
		dup2(path->fd_in, STDIN_FILENO);
		close(path->fd_in);
	}
	if (path->fd_out != 1)
	{
		dup2(path->fd_out, STDOUT_FILENO);
		close(path->fd_out);
	}
	
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ex_strcmp("cd", cmd) == 0
		|| ex_strcmp("pwd", cmd) == 0
		|| ex_strcmp("echo", cmd) == 0
		|| ex_strcmp("echo", cmd) == 0
		|| ex_strcmp("export", cmd) == 0
		|| ex_strcmp("env", cmd) == 0
		|| ex_strcmp("unset", cmd) == 0
		|| ex_strcmp("exit", cmd) == 0)
		return (1);
	return (0);
}

int	ambigous_case(t_command *command, t_path *path, int i)
{
	int	pos;

	pos = -1;
	if (command->is_ambiguous != -1)
		pos = command->is_ambiguous - ft_strslen(command->cmd);
	if (pos == i + 1)
	{
		print_error(command->ambiguous_file, NULL, "ambiguous redirect");
		if (path->is_forked)
			exit(1);
		else
			return (1);
	}
	return (0);
}
void env_dataclear(t_env **head)
{
    t_env *tmp;
    t_env *ptr;
    tmp = *head;
    while(tmp)
    {
        ptr = tmp;
        tmp = tmp->next;
        free(ptr->key);
		free(ptr->value);
        free(ptr);
    }
    *head = NULL;
}

void envp_error(t_env *env)
{
	write(2, "Allocation Error\n", 17);
	free_envs(env);
	exit(1);
}
void fd_error(t_path *path, t_env *env)
{
	write(2, "File descriptor Error\n", 22);
	if (path->fd_in >= 0)
        close(path->fd_in);
	if (path->fd_out >= 0)
        close(path->fd_out);
	my_malloc(0, 0);
	free_envs(env);
	free(path->main_path);
	free(path->pwd);
	free(path);
	exit(1);
}

void malloc_error(t_path *path, t_env *env)
{
	write(2, "Allocation Error\n", 17);
	if (path->fd_in >= 0)
        close(path->fd_in);
	if (path->fd_out >= 0)
        close(path->fd_out);
	my_malloc(0, 0);
	free_envs(env);
	if(path->main_path)
    	free(path->main_path);
    if (path->pwd)
        free(path->pwd);
	if (path)
		free(path);
	exit(1);
}

char	*special_join(char *s1, char *s2) // a join without free
{
	char	*res;
	int		length;
	int		i;
	int		j;

	if (!s1)
		return (ex_strdup(s2));
	if (!s2)
		return (s1);
	length = ex_strlen(s1) + ex_strlen(s2);
	res = (char *)malloc(sizeof(char) * length + 1);
	if (!res)
		return (0);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	return (res);
}

char	*my_get_key(char *str)
{
	char	*key;
	int		i;

	i = 0;
	key = (char *)my_malloc(sizeof(char) * ft_strlen(str) + 1, 1);
	if (!key)
		return (NULL);
	while (str[i] && str[i] != '=' && str[i] != '+')
	{
		key[i] = str[i];
		i++;
	}
	if (str[i] == '+' && str[i + 1] == '+')
		return (NULL);
	key[i] = '\0';
	return (key);
}
char	*my_get_value(char *str)
{
	char	*value;
	int		i;
	int		j;

	i = 0;
	j = 0;
	value = (char *)my_malloc(sizeof(char) * ft_strlen(str) + 1, 1);
	if (!value)
		return (NULL);
	while (str[j] && str[j] != '=')
		j++;
	if (str[j] != '=')
		return (NULL);
	j++;
	while (str[j])
	{
		value[i] = str[j];
		i++;
		j++;
	}
	value[i] = '\0';
	return (value);
}

char	*my_get_sep(char *str)
{
	char	*sep;
	int		i;
	int		j;

	i = 0;
	j = 0;
	sep = (char *)my_malloc(sizeof(char) * ft_strlen(str) + 1, 1);
	if (!sep)
		return (NULL);
	while (str[j] && str[j] != '=' && str[j] != '+')
		j++;
	if (str[j] == '=')
	{
		sep[i++] = '=';
		sep[i++] = '\0';
	}
	else if (str[j] == '+' && str[j + 1] == '=')
	{
		sep[i++] = '+';
		sep[i++] = '=';
		sep[i] = '\0';
	}
	else
		return (NULL);
	return (sep);
}




