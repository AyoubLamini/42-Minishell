/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:55:50 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/09/23 11:03:48 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../exec/minishell_exec.h"

static t_env *empty_envs(void)
{
	t_env *env_vars;
	char *path;
	char *pwd;
	
	
	env_vars = NULL;
	path = ft_strdup("/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin");
	pwd = (char *)malloc(sizeof(char) * PATH_MAX);
	getcwd(pwd, PATH_MAX);
	add_env_back(&env_vars, new_variable(ft_strdup("PATH"), path));
	add_env_back(&env_vars, new_variable(ft_strdup("PWD"), pwd));
	add_env_back(&env_vars, new_variable(ft_strdup("SHLVL"), ft_strdup("1")));
	add_env_back(&env_vars, new_variable(ft_strdup("OLDPWD"), NULL));
	return (env_vars);
}
t_env   *full_envs(char **env)
{
	int		i;
	t_env	*node;
	t_env	*env_vars;

	if (*env == NULL) // if env is empty
		return (empty_envs());
	env_vars = NULL;
	i = 0;
	while (env[i])
	{
		node = new_variable(get_str(env[i], "key"), get_str(env[i], "value")); // get key and value and send to node
		add_env_back(&env_vars, node);
		i++;
	}
	if (!get_env_key(env_vars ,"OLDPWD"))
		add_env_back(&env_vars, new_variable(ft_strdup("OLDPWD"), NULL)); // I add this, by default OLDPWD var is not included in envp
	return (env_vars);
}

// char	*expanding(t_env *envs, char *old_cmd)
// {
// 	char	*expanded_cmd;
// 	char	**cmd;
// 	int		i;

// 	i = 0;
// 	cmd = expanding_split(old_cmd);
		
// 	(void)envs;
	
// 	return (new_cmd); 
// }

int	check_quotes_existed(char *old_cmd)
{
	int	i;

	i = 0;
	while (old_cmd[i] && old_cmd[i] != '"' && old_cmd[i] != '\'')
		i++;
	if (old_cmd[i] == '\'')
		return (1);
	if (old_cmd[i] == '"')
		return (2);
	return (0);
}

int	get_last_quote_pos(char	*old_cmd)
{
	int	len;

	len = ft_strlen(old_cmd);
	while (len && old_cmd[len] != '"')
		len--;
	return (len);
}

char	*double_quotes(t_env *envs, char *str, t_path path)
{
	int		i;
	char	*res;
	int		start;
	char	*key;
	res= NULL;
	start = 0;
	i = 0;
	if (str[0] == '"')
		str++;
	while (str[i] && str[i] != '"')
	{
		start = i;
		while (str[i] && str[i] != '$' && str[i] != '"')
			i++;
		key = ft_substr(str, start, i - start);
		res = ft_strjoin(res, key);
		if (str[i] == '$')
		{
			i++;
			start = 1;
			while (str[i] == '$' && str[i])
			{
				start++;
				i++;
			}
			if (start % 2 == 0)
			{
				start = start / 2;
				while (start)
				{
					res = ft_strjoin(res, "$");
					start--;
				}
			}
			else
			{
				start = start / 2;
				while (start >= 1)
				{
					res = ft_strjoin(res, "$");
					start--;
				}
				if (str[i] == '?')
				{
					res = ft_strjoin(res, ft_itoa(path.exit_status));
					i++;
				}
				else if (ft_isdigit(str[i]))
					i++;
				else
				{
					if (!ft_isalpha(str[i]))
						res = ft_strjoin(res, "$");
					start = i;
					while (ft_isalnum(str[i]) && str[i])
						i++;
					key = ft_substr(str, start, i - start);
					if (get_env_variable(envs, key))
						res = ft_strjoin(res, get_env_variable(envs, key));
				}
			}
		}
	}
	res = ft_strjoin(res, "\0");
	
	// printf("res: %s\n", res);
	return (res);
}

char	*single_quotes_process(char *str)
{
	int		i;
	char	*res;
	
	res = NULL;
	i = 0;
	str++;
	while (str[i] && str[i] != '\'')
		i++;
	if (str[i] == '\'')
		str[i] = '\0';
	res = ft_strjoin(res, str);
	// printf("res: %s\n", res);
	
	return (res);
}

int	check_is_joinable(char **cmd, int index)
{
	int	i;
	int	nbr;
	
	i = 0;
	while (cmd[index][i])
	{
		if (cmd[index][i] == '$')
		{
			i++;
			nbr = 1;
			while (cmd[index][i] && cmd[index][i] == '$')
			{
				nbr++;
				i++;
			}
			if (cmd[index + 1] != NULL)
			{
				if (nbr % 2 != 0 && cmd[index][i] == '\0' && (cmd[index + 1][0] == '"' || cmd[index + 1][0] == '\''))
					return (1);	
			}
		}
		else
			i++;
	}
	return (0);
}

int	check_will_splited(t_env *envs, char *str, t_path path)
{
	int	i;
	int	check;
	char	**tmp;
	char	*temp;
	
	check = 0;

	i = 0;
	if (str[i] == '"' || str[i] == '\'')
		return (0);
	while (str[i] && str[i] != '"')
	{
		if (str[i] == '$' && ft_isalpha(str[i + 1]))
			check = 1;
		i++;
	}
	temp = double_quotes(envs, str, path);
	tmp = ft_split(temp, ' ');
	if (check == 1 && ft_strslen(tmp) > 1)
		return (1);
	return (0);
}

int	ft_check_space_in_cmd(char *str)
{
	int	i;
	int	begining;
	int	len;

	if (str[0] == '\0')
		return (0);
	begining = 0;
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (i > 0)
		begining = 1;
	len = ft_strlen(str) - 1;
	i = len;
	// printf("len %d\n", len);
	while (i && ft_isspace(str[i]))
		i--;
	// printf("i : %d\n", i);
	if (len != i && begining == 1)
	{
		return (2);
	}
	else if (len != i)
		return (1);
	else
	{
		// printf("yesss\n");
		return (0);
	}
}

int	is_only_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	if (str[i] == '\0')
		return (1);
	return (0);
}

char	**expanding_cmd(t_env *envs, char *old_cmd, t_path path)
{
	char	**cmd;
	char	*tmp;
	char	**res;
	char	**temp;
	int		i;
	int		index;
	char	*tmp1;
	res = NULL;
	temp = NULL;
	tmp = NULL;
	i = 0;
	cmd = expanding_split(old_cmd);
	while (cmd[i])
	{
		if (cmd[i][0] == '\'')
		{
			tmp = single_quotes_process(cmd[i]);
			index = ft_strslen(res);
			if (!res)
				res = join_double_strs_with_str(res, tmp);
			else
			{
				res[index - 1] = ft_strjoin(res[index - 1], tmp);
				res[index] = 0;
			}
		}
		else
		{
			if (check_is_joinable(cmd, i))
			 	cmd[i][ft_strlen(cmd[i]) - 1] = '\0';
			tmp = double_quotes(envs, cmd[i], path);
			// printf("cmd: |%s|\n", cmd[i]);
			if (cmd[i][0] == '"' || (cmd[i][0] != '"' && !is_only_spaces(tmp)))
			{
				// printf("tmp : %s\n", tmp);
				// printf("i: %d\n", i);
				if (i > 0)
					tmp1 = double_quotes(envs, cmd[i - 1], path);
				else
					tmp1 = tmp;
				// printf("cmd: %s\n", cmd[i]);
				if (check_will_splited(envs, cmd[i], path) == 1 && tmp && tmp[0] != '\0')
				{
					// printf("here\n");
					temp = ft_split(tmp, ' ');
					res = join_two_double_strs(res, temp);
				}
				else 
				{
					// printf("here\n");
					if (!res)
					{
						// printf("mra lwla\n");
						//printf("here\n");
						res = join_double_strs_with_str(res, tmp);
					}
					else
					{
						// printf("tmp: |%s|\n", tmp);
						// printf("tmp1: |%s|\n", tmp1);
						if (ft_check_space_in_cmd(tmp1) > 0)
						{
							res = join_double_strs_with_str(res, tmp);	
						}
						else
						{
							index = ft_strslen(res);
							res[index - 1] = ft_strjoin(res[index - 1], tmp);
							res[index] = 0;
						}
					}
				}
			}
			else
				res = join_double_strs_with_str(res, tmp);
		}
		i++;
	}
	// printstrs(res);
	return (free_strs(cmd), res);
}

char	**expanding_red(t_env *envs, char *old_cmd, t_path path)
{
	char	**cmd;
	char	*tmp;
	char	**res;
	char	**temp;
	int		i;
	int		index;
	char	*tmp1;
	res = NULL;
	temp = NULL;
	tmp = NULL;
	i = 0;
	cmd = expanding_split(old_cmd);
	while (cmd[i])
	{
		if (cmd[i][0] == '\'')
		{
			tmp = single_quotes_process(cmd[i]);
			index = ft_strslen(res);
			if (!res)
				res = join_double_strs_with_str(res, tmp);
			else
			{
				res[index - 1] = ft_strjoin(res[index - 1], tmp);
				res[index] = 0;
			}
		}
		else
		{
			if (check_is_joinable(cmd, i))
			 	cmd[i][ft_strlen(cmd[i]) - 1] = '\0';
			tmp = double_quotes(envs, cmd[i], path);
			if (i > 0)
				tmp1 = double_quotes(envs, cmd[i - 1], path);
			else
				tmp1 = tmp;
			// printf("check : %d\n", check_will_splited(cmd[i]));
			if (!ft_strcmp(cmd[i], "*") && !cmd[i + 1])
			{
				printf("Minishell : ambigous redirect\n");
				exit(1);
			}
			if (check_will_splited(envs, cmd[i], path) == 1 && tmp && tmp[0] != '\0')
			{
				// printf("cmd[]: %s\n", cmd[i + 1]);
				//exit(1);
				temp = ft_split(tmp, ' ');
				//printstrs(temp);
				if (ft_strslen(temp) > 1 || (ft_strslen(temp) == 1 && !ft_strcmp(temp[0],"*") && !cmd[i + 1]))
				{
					printf("Minishell : ambigous redirect\n");
					exit(1);
				}
				res = join_two_double_strs(res, temp);
			}
			else
			{
				// printf("here\n");
				//printf("error: ambiguous!\n");
				if (!res)
				{
					// printf("here\n");
					// printf("tmp: %s\n", tmp);
					// printf("cmd[i]: %s\n", cmd[i]);
					if (tmp[0] == '\0' && !cmd[i + 1])
					{
						printf("Minishell : ambigous redirect\n");
						exit(1);
					}
					res = join_double_strs_with_str(res, tmp);
				}
				else
				{
					//printf("tmp: %s\n", tmp);
					if (ft_check_space_in_cmd(tmp1) == 1)
					{
						// printf("yesssss\n");
						res = join_double_strs_with_str(res, tmp);	
					}
					else
					{
						index = ft_strslen(res);
						res[index - 1] = ft_strjoin(res[index - 1], tmp);
						res[index] = 0;
					}
				}
			}
		}
		i++;
	}

	return (free_strs(cmd), res);
}