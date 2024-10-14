/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:55:50 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/14 13:34:19 by ybouyzem         ###   ########.fr       */
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

static void	count_dollars(t_vars *vars, char *str)
{
	vars->i += 1;
	vars->start = 1;
	while (str[vars->i] == '$' && str[vars->i])
	{
		vars->start++;
		vars->i++;
	}
}

static void	add_string(t_vars *vars, char *str)
{
	vars->start = vars->i;
	while (str[vars->i] && str[vars->i] != '$' && str[vars->i] != '"')
			vars->i++;
	vars->key = ft_substr(str, vars->start, vars->i - vars->start);
	vars->new = ft_strjoin(vars->new, vars->key);
}

// static void add_expanded(t_vars *vars, t_env *envs, char *str)
// {
// 	if (!ft_isalpha(str[vars->i]) && str[vars->i] != '_' && str[vars->i] != '-')
// 		vars->new = ft_strjoin(vars->new, "$");
// 	vars->start = vars->i;
// 	while ((ft_isalnum(str[vars->i]) || str[vars->i] == '_' || str[vars->i] == '-') && str[vars->i])
// 		vars->i++;
// 	vars->key = ft_substr(str, vars->start, vars->i - vars->start);
// 	vars->value = get_env_variable(envs, vars->key);
// 	if (vars->value)
// 		vars->new = ft_strjoin(vars->new, vars->value);
// }

static void exit_status_case(t_vars *vars, t_path *path, int is_pipe)
{
	if (g_last_signal == SIGINT)
	{
		vars->new = ft_strjoin(vars->new, "1");
		g_last_signal = 0;
	}
	else if (is_pipe > 0)
		vars->new = ft_strjoin(vars->new, "0");
	else
	{
		vars->value = ft_itoa(path->exit_status);
		vars->new = ft_strjoin(vars->new, vars->value);
	}
	vars->i++;
}

static void last_command_case(t_vars *vars, t_env *envs, t_path *path, int is_pipe)
{
	if (is_pipe > 0)
		vars->new = ft_strjoin(vars->new, "\0");
	else
	{
		vars->value = get_env_variable(envs, "_");
		if (vars->value)
			vars->new = ft_strjoin(vars->new, vars->value);
	}
}

void	add_dollars(t_vars *vars, int c)
{
	vars->start = vars->start / 2;
	if (c == 1)
	{
		while (vars->start)
		{
			vars->new = ft_strjoin(vars->new, "$");
			vars->start--;
		}
	}
	else if (c == 2)
	{
		while (vars->start >= 1)
		{
			vars->new = ft_strjoin(vars->new, "$");
			vars->start--;
		}
	}
}

void double_quotes_helper2(t_env *envs, t_vars *vars, t_path *path, char *str)
{
	if (str[vars->i] == '?')
		exit_status_case(vars, path, vars->is_pipe);
	else if (ft_isdigit(str[vars->i]))
		vars->i++;
	else if (str[vars->i] == '_' && !ft_isalnum(str[vars->i + 1]))
	{
		last_command_case(vars, envs, path, vars->is_pipe);
		vars->i++;
	}
	else
	{
		if (!ft_isalnum(str[vars->i]) && str[vars->i] != '_')
			vars->new = ft_strjoin(vars->new, "$");
		vars->start = vars->i;
		while ((ft_isalnum(str[vars->i]) || str[vars->i] == '_') && str[vars->i])
			vars->i++;
		vars->key = ft_substr(str, vars->start, vars->i - vars->start);
		vars->value = get_env_variable(envs, vars->key);
		if (vars->value)
			vars->new = ft_strjoin(vars->new, vars->value);
	}
}

char	*double_quotes_process(t_env *envs, char *str, t_path *path, int is_pipe)
{
	t_vars	vars;
	vars = ft_initialize_vars();
	vars.is_pipe = is_pipe;
	if (str[0] == '"')
		str++;
	while (str[vars.i] && str[vars.i] != '"')
	{
		add_string(&vars, str);
		if (str[vars.i] == '$')
		{
			count_dollars(&vars, str);
			if (vars.start % 2 == 0)
				add_dollars(&vars, 1);
			else
			{
				add_dollars(&vars, 2);
				double_quotes_helper2(envs, &vars, path, str);
			}
		}
	}
	vars.new = ft_strjoin(vars.new, "\0");
	return (free_str(vars.key), free_str(vars.value), vars.new);
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
			vars.tmp = double_quotes_process(envs, cmd[vars.i], path, is_pipe);
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
			vars.tmp = double_quotes_process(envs, cmd[vars.i], path, -1);
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
