/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:55:50 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/29 06:54:55 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../exec/minishell_exec.h"

t_env   *full_envs(char **env)
{
	int		i;
	t_env	*node;
	t_env	*env_vars;

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

char	*double_quotes(t_env *envs, char *str)
{
	int		i;
	char	*res;
	int		start;
	char	*key;
	res= NULL;
	start = 0;
	i = 0;
	if (*str == '"')
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
				if (ft_isdigit(str[i]))
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

int	check_will_splited(char **old_cmd)
{
	int	i;
	int	j;
	int	check;

	check = 0;
	j = 0;
	i = 0;
	while (old_cmd[i])
	{
		if (old_cmd[i][0] == '"' || old_cmd[i][0] == '\'')
			return (0);
		i++;
	}
	i = 0;
	while (old_cmd[0][i])
	{
		if (old_cmd[0][i] == '$' && ft_isalpha(old_cmd[0][i + 1]))
			check = 1;
		i++;
	}
	if (check == 1)
		return (1);
	return (0);
}

// int	count_cmd_split_words(char *new_cmd)
// {
// 	int	i;
// 	int	nbr;

// 	nbr = 0;
// 	i = 0;
// 	while (new_cmd[i])
// 	{
// 		while (ft_isspace(new_cmd[i]) && new_cmd[i])
// 			i++;
// 		if (new_cmd[i] != '\0')
// 			nbr++;
// 		while (!ft_isspace(new_cmd[i]) && new_cmd[i])
// 			i++;
// 	}
// 	return (nbr);
// }

// char	**cmd_split(char *new_cmd)
// {
// 	int		i;
// 	char	**res;
// 	int		start;
// 	char	*tmp;
// 	int		index;
// 	start = 0;
// 	index = 0;
// 	i = 0;
// 	// printf("lw: %d\n", count_cmd_split_words(new_cmd));
// 	res = (char **)malloc(sizeof(char *) * (count_cmd_split_words(new_cmd) + 1));
// 	if (!res)
// 		return (NULL);
// 	while (new_cmd[i])
// 	{
		
// 		while (ft_isspace(new_cmd[i]) && new_cmd[i])
// 			i++;
// 		if (new_cmd[i] != '\0')
// 		{
// 			tmp = ft_substr(new_cmd, start, i - start);
// 			res[index++] = tmp;
// 		}
// 		while (ft_isspace(new_cmd[i]) && new_cmd[i])
// 			i++;
// 	}
// 	res[index] = NULL;
// //	printstrs(res);
// 	return (res);
// }

char	**expanding_cmd(t_env *envs, char *old_cmd)
{
	char	**cmd;
	char	*new_cmd;
	char	*tmp;
	char	**res;
	int		i;
	int		j;

	j = 0;
	tmp = NULL;
	new_cmd = NULL;
	i = 0;
	cmd = expanding_split(old_cmd);
	while (cmd[i])
	{
		if (cmd[i][0] == '\'')
		{
			tmp = single_quotes_process(cmd[i]);
		}
		else
		{
			if (check_is_joinable(cmd, i))
			 	cmd[i][ft_strlen(cmd[i]) - 1] = '\0';
			
			tmp = double_quotes(envs, cmd[i]);
		}

		new_cmd = ft_strjoin(new_cmd, tmp);
		i++;
	}
	new_cmd = ft_strjoin(new_cmd, "\0");
	//printf("new_cmd: %s\n", new_cmd);
	// if (new_cmd[0] == '\0')
	// {
	// 	res = (char **)malloc(sizeof(char *) * 2);
	// 	if (!res)
	// 		return (NULL);
	// 	res[0] = "";
	// 	res[1] = NULL;
	// 	printstrs(res);
	// 	return (res);
	// }
	if (check_will_splited(cmd))
		res = ft_split(new_cmd, ' ');
	else
	{
		res = (char **)malloc(sizeof(char *) * 2);
		if (!res)
			return (NULL);
		res[0] = new_cmd;
		res[1] = NULL;
	}
	return (res);
}

char	*expanding_red(t_env *envs, char *old_cmd)
{
	char	**cmd;
	char	*new_cmd;
	char	*tmp;
	int		i;
	
	tmp = NULL;
	new_cmd = NULL;
	i = 0;
	cmd = expanding_split(old_cmd);
	while (cmd[i])
	{
		if (cmd[i][0] == '\'')	
			tmp = single_quotes_process(cmd[i]);
		else
		{
			if (check_is_joinable(cmd, i))
				cmd[i][ft_strlen(cmd[i]) - 1] = '\0';
			tmp = double_quotes(envs, cmd[i]);
		}
		new_cmd = ft_strjoin(new_cmd, tmp);
		i++;
	}
	new_cmd = ft_strjoin(new_cmd, "\0");
	// printf("new_cmd: %s\n", new_cmd);
	return (new_cmd);
}
