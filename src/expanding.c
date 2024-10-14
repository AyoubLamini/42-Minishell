/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:55:50 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/14 13:47:48 by ybouyzem         ###   ########.fr       */
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


char	**expanding_cmd(t_env *envs, char *old_cmd, t_path *path, int is_pipe)
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
			vars.tmp = double_quotes_p(envs, cmd[vars.i], path, is_pipe);
			if ((vars.tmp[0] == '\0' && cmd[vars.i][0] == '"') || vars.tmp[0] != '\0')
			{
				if (cmd[vars.i][0] == '"' || (cmd[vars.i][0] != '"' && !is_only_spaces(vars.tmp)))
				{
					if (vars.tmp && vars.tmp[0] != '\0' && check_will_splited(envs, cmd, vars.i) == 1 && cmd[vars.i][0] != '"')
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
	return (vars.res);
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
			vars.tmp = double_quotes_p(envs, cmd[vars.i], path, -1);
			if ((vars.tmp[0] == '\0' && cmd[vars.i][0] == '"') || vars.tmp[0] != '\0')
			{ 
				if (cmd[vars.i][0] == '"' || (cmd[vars.i][0] != '"' && !is_only_spaces(vars.tmp)))
				{
					if (vars.tmp && vars.tmp[0] == '*' && cmd[vars.i][0] != '"' && vars.tmp[1] == '\0')
					{
						node->is_ambiguous = pos;
						node->ambiguous_file = old_cmd;
					}
					else if (vars.tmp && vars.tmp[0] != '\0' && check_will_splited_ambg(envs, cmd, vars.i) == 1 && cmd[vars.i][0] != '"')
					{
						node->is_ambiguous = pos;
						node->ambiguous_file = old_cmd;
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
