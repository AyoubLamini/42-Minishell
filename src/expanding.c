/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:55:50 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/08 06:55:56 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../exec/minishell_exec.h"



int	get_last_quote_pos(char	*old_cmd)
{
	int	len;

	len = ft_strlen(old_cmd);
	while (len && old_cmd[len] != '"')
		len--;
	return (len);
}

char	*double_quotes_process(t_env *envs, char *str, t_path *path)
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
				if (str[i] == '_')
				{
					res = ft_strjoin(res, get_env_value(envs, "_"));
					i++;
				}
				else if (str[i] == '?')
				{
					res = ft_strjoin(res, ft_itoa(path->exit_status));
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


char	**single_quotes(t_env *envs, char **cmd, int i, char **res, int *index)
{
	char	*tmp;
	char	**temp;

	temp = NULL;
	tmp = NULL;
	tmp = single_quotes_process(cmd[i]);
	*index = ft_strslen(res);
	
	if (tmp && tmp[0] != '\0' && check_will_splited(envs, cmd, i) == 1 )
	{
		temp = ft_split(tmp, ' ');
		res = join_two_double_strs(res, temp);
	}
	else
	{	
		if (!res)
		{
			res = join_double_strs_with_str(res, tmp);
		}
		else
		{
			res[*index - 1] = ft_strjoin(res[*index - 1], tmp);
			res[*index] = 0;
		}
	}
	return (res);
}


char	**expanding_cmd(t_env *envs, char *old_cmd, t_path *path)
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
	tmp1 = NULL;
	i = 0;
	cmd = expanding_split(old_cmd);
	while (cmd[i])
	{
		if (cmd[i][0] == '\'')
			res = single_quotes(envs, cmd, i , res, &index);
		else
		{
			if (check_is_joinable(cmd, i))
			 	cmd[i][ft_strlen(cmd[i]) - 1] = '\0';
			tmp = double_quotes_process(envs, cmd[i], path);
			if ((tmp[0] == '\0' && cmd[i][0] == '"') || tmp[0] != '\0')
			{ 
				if (cmd[i][0] == '"' || (cmd[i][0] != '"' && !is_only_spaces(tmp)))
				{
					if (tmp && tmp[0] != '\0' && check_will_splited(envs, cmd, i) == 1 )
					{
						temp = ft_split(tmp, ' ');
						res = join_two_double_strs(res, temp);
					}
					else
					{
						if (!res)
							res = join_double_strs_with_str(res, tmp);
						else
						{
								index = ft_strslen(res);
								res[index - 1] = ft_strjoin(res[index - 1], tmp);
								res[index] = 0;
						}
					}
				}
				else
					res = join_double_strs_with_str(res, tmp);
			}
		}
		i++;
	}
	return (free_strs(cmd), res);
}


char	**expanding_red(t_env *envs, char *old_cmd, t_path *path)
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
			tmp = double_quotes_process(envs, cmd[i], path);
			if (i > 0)
				tmp1 = double_quotes_process(envs, cmd[i - 1], path);
			else
				tmp1 = tmp;
			// printf("check : %d\n", check_will_splited(cmd[i]));
			if (!ft_strcmp(cmd[i], "*") && !cmd[i + 1])
			{
				printf("Minishell : ambigous redirect\n");
				exit(1);
			}
			if (check_will_splited(envs, cmd, i) == 1 && tmp && tmp[0] != '\0')
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
