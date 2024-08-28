/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:55:50 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/28 03:46:41 by ybouyzem         ###   ########.fr       */
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

char	*double_quotes(t_env *envs, char *str, int pid)
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
			while (str[i] == '$')
			{
				start++;
				i++;
			}
			if (start % 2 == 0)
			{
				start = start / 2;
				while (start)
				{
					res = ft_strjoin(res, ft_itoa(pid));
					start--;
				}
			}
			else
			{
				start = start / 2;
				while (start >= 1)
				{
					res = ft_strjoin(res, ft_itoa(pid));
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

// char	*double_quotes_process(t_env *envs, char *old_cmd, char *new_cmd, int pid, int i)
// {
// 	int		start;
// 	int		pos;
// 	char	*key;
// 	int		double_quote;
// 	int		single_quote;



// 	single_quote = 0;
// 	double_quote = 0;
// 	pos = 0;
// 	int	last_quote;
// 	printf("here\n");
// 	last_quote = get_last_quote_pos(old_cmd);
// 	while (old_cmd[i])
// 	{
// 		ft_check_quotes(&single_quote, &double_quote, old_cmd[i]);
// 		while (old_cmd[i] == '"' && old_cmd[i])
// 			i++;
// 		while (old_cmd[i] != '$' && old_cmd[i])
// 		{
// 			if (old_cmd[i] == '\'' && i < last_quote)
// 				new_cmd[pos++] = old_cmd[i];
// 			else if (old_cmd[i] != '"')
// 				new_cmd[pos++] = old_cmd[i];
// 				i++;
// 		}
// 		if (old_cmd[i] == '$')
// 		{
// 			i++;
// 			start = 1;
// 			if ((old_cmd[i] != '$' && !ft_isalnum(old_cmd[i])) || old_cmd[i] == '\0')
// 				new_cmd[pos++] = '$';
// 			while (old_cmd[i] == '$')
// 			{
// 				start++;
// 				i++;
// 			}
// 			if (start % 2 == 0)
// 			{
// 				start = start / 2;
// 				while (start)
// 				{
// 					new_cmd = ft_strjoin(new_cmd, ft_itoa(pid));
// 					start--;
// 					pos = ft_strlen(new_cmd);
// 				}
// 			}
// 			else
// 			{
// 				start = start / 2;
// 				while (start >= 1)
// 				{
// 					new_cmd = ft_strjoin(new_cmd, ft_itoa(pid));
// 					start--;
// 					pos = ft_strlen(new_cmd);
// 				}
// 				if (!ft_isalpha(old_cmd[i]))
// 					new_cmd[pos++] = '$';
// 				start = i;
// 				while (ft_isalnum(old_cmd[i]) && old_cmd[i])
// 					i++;
// 				key = ft_substr(old_cmd, start, i - start);
// 				if (get_env_variable(envs, key))
// 					new_cmd = ft_strjoin(new_cmd, get_env_variable(envs, key));
// 			}
// 				pos = ft_strlen(new_cmd);
// 		}
// 	}
// 	new_cmd[pos] = '\0';
// 	return (new_cmd);
// }

// char	*expanding(t_env *envs, char *old_cmd, int pid)
// {
// 	int		i;
// 	int		pos;
// 	char	*new_cmd;
	
// 	pos  = 0;
// 	pid = getpid();
// 	i = 0;
// 	char **key;
// 		key = expanding_split(old_cmd);

// 	new_cmd = (char *)malloc(sizeof(char) * ((ft_strlen(old_cmd) * 5) + 1));
// 	if (!new_cmd)
// 		return (new_cmd);
// 	if (check_quotes_existed(old_cmd) == 1)
// 		new_cmd[pos] = '\0';
// 	else
// 		return (double_quotes_process(envs, old_cmd, new_cmd, pid, i));
// 	return (new_cmd);
// }

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
			if (nbr % 2 != 0 && cmd[index][i + 1] == '\0' && (cmd[index + 1][0] == '"' || cmd[index + 1][0] == '\''))
				return (1);
		}
		else
			i++;
	}
	return (0);
}

char	*expanding(t_env *envs, char *old_cmd, int pid)
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
			{
				cmd[i][ft_strlen(cmd[i]) - 1] = '\0';
			}
			tmp = double_quotes(envs, cmd[i], pid);	
		}
		new_cmd = ft_strjoin(new_cmd, tmp);
		i++;
	}
	new_cmd = ft_strjoin(new_cmd, "\0");
	// printf("new_cmd: %s\n", new_cmd);
	return (new_cmd);
}
