/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:55:50 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/09 23:23:13 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"



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
					if (g_last_signal == 2)
					{
						res = ft_strjoin(res, "1");
						g_last_signal = 0;		
					}
					else
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
	t_vars	vars;
	
	vars = ft_initialize_vars();
	char	**cmd;
	char	**temp;
	temp = NULL;
	cmd = expanding_split(old_cmd);
	while (cmd[vars.i])
	{
		if (cmd[vars.i][0] == '\'')
			vars.res = single_quotes(envs, cmd, vars.i , vars.res, &vars.index);
		else
		{
			if (check_is_joinable(cmd, vars.i))
			 	cmd[vars.i][ft_strlen(cmd[vars.i]) - 1] = '\0';
			vars.tmp = double_quotes_process(envs, cmd[vars.i], path);
			if ((vars.tmp[0] == '\0' && cmd[vars.i][0] == '"') || vars.tmp[0] != '\0')
			{ 
				if (cmd[vars.i][0] == '"' || (cmd[vars.i][0] != '"' && !is_only_spaces(vars.tmp)))
				{
					if (vars.tmp && vars.tmp[0] != '\0' && check_will_splited(envs, cmd, vars.i) == 1 )
					{
						temp = ft_split(vars.tmp, ' ');
						vars.res = join_two_double_strs(vars.res, temp);
					}
					else
					{
						if (!vars.res)
							vars.res = join_double_strs_with_str(vars.res, vars.tmp);
						else
						{
								vars.index = ft_strslen(vars.res);
								vars.res[vars.index - 1] = ft_strjoin(vars.res[vars.index - 1], vars.tmp);
								vars.res[vars.index] = 0;
						}
					}
				}
				else
					vars.res = join_double_strs_with_str(vars.res, vars.tmp);
			}
		}
		vars.i++;
	}
	return (free_strs(cmd), vars.res);
}


char	**expanding_red(t_command *node, t_env *envs, char *old_cmd, t_path *path, int pos)
{
	t_vars	vars;
	
	vars = ft_initialize_vars();
	char	**cmd;
	char	**temp;
	temp = NULL;
	cmd = expanding_split(old_cmd);
	while (cmd[vars.i])
	{
		if (cmd[vars.i][0] == '\'')
			vars.res = single_quotes(envs, cmd, vars.i , vars.res, &vars.index);
		else
		{
			if (check_is_joinable(cmd, vars.i))
			 	cmd[vars.i][ft_strlen(cmd[vars.i]) - 1] = '\0';
			vars.tmp = double_quotes_process(envs, cmd[vars.i], path);
			if ((vars.tmp[0] == '\0' && cmd[vars.i][0] == '"') || vars.tmp[0] != '\0')
			{ 
				if (cmd[vars.i][0] == '"' || (cmd[vars.i][0] != '"' && !is_only_spaces(vars.tmp)))
				{
					if (vars.tmp && vars.tmp[0] == '*' && vars.tmp[1] == '\0')
					{
						node->is_ambiguous = pos;
						node->ambiguous_file = old_cmd;
					}
					if (vars.tmp && vars.tmp[0] != '\0' && check_will_splited(envs, cmd, vars.i) == 1 )
					{
						node->is_ambiguous = pos;
						node->ambiguous_file = old_cmd;
						temp = ft_split(vars.tmp, ' ');
						vars.res = join_two_double_strs(vars.res, temp);
					}
					else
					{
						if (!vars.res)
							vars.res = join_double_strs_with_str(vars.res, vars.tmp);
						else
						{
								vars.index = ft_strslen(vars.res);
								vars.res[vars.index - 1] = ft_strjoin(vars.res[vars.index - 1], vars.tmp);
								vars.res[vars.index] = 0;
						}
					}
				}
				else
					vars.res = join_double_strs_with_str(vars.res, vars.tmp);
			}
			else
			{
				node->is_ambiguous = pos;
				node->ambiguous_file = old_cmd;
			}
		}
		vars.i++;
	}
	return (free_strs(cmd), vars.res);
}
