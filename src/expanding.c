/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:55:50 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/25 15:59:37 by alamini          ###   ########.fr       */
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

int	ft_isspecial(char c)
{
	if (!ft_isalnum(c))
		return (1);
	return (0);
}

char	*expanding(t_env *envs, char *old_cmd, int pid)
{
	int	i;
	int	pos;
	char	*new_cmd;
	(void)envs;
	int	single_quote;
	int	double_quote;
	int		start;
	char	*key;

	pid = getpid();
	single_quote = 0;
	double_quote = 0;
	i = 0;
	pos = 0;
	new_cmd = (char *)malloc(sizeof(char) * (ft_strlen(old_cmd) + 1));
	if (!new_cmd)
		return (new_cmd);
	while (old_cmd[i] != '\'' && old_cmd[i] != '"' && old_cmd[i])
		new_cmd[pos++] = old_cmd[i++];
	if (old_cmd[i] == '"')
	{	
		while (old_cmd[i])
		{
			while (old_cmd[i] == '"' && old_cmd[i])
				i++;
			while (old_cmd[i] != '$' && old_cmd[i])
			{
				if (old_cmd[i] != '"')
					new_cmd[pos++] = old_cmd[i];
					i++;
			}
			if (old_cmd[i] == '$')
			{
				i++;
				start = 1;
				if (old_cmd[i] != '$' && !ft_isalnum(old_cmd[i]))
					new_cmd[pos++] = '$';
				while (old_cmd[i] == '$')
				{
					start++;
					i++;
				}
				if (start % 2 == 0)
				{
					start = start / 2;
					while (start)
					{
						new_cmd = ft_strjoin(new_cmd, ft_itoa(pid));
						start--;
						pos = ft_strlen(new_cmd);
					}
				}
				else
				{
					start = start / 2;
					while (start)
					{
						new_cmd = ft_strjoin(new_cmd, ft_itoa(pid));
						start--;
						pos = ft_strlen(new_cmd);	
					}
					start = i;
					while (ft_isalnum(old_cmd[i]) && old_cmd[i])
						i++;
					key = ft_substr(old_cmd, start, i - start);
					if (get_env_variable(envs, key))
						new_cmd = ft_strjoin(new_cmd, get_env_variable(envs, key));
				}
				pos = ft_strlen(new_cmd);
			}
		}
	}
	new_cmd[pos] = '\0';
	return (new_cmd);
}