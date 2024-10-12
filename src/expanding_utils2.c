/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 05:46:27 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/12 06:25:16 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"


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
	add_env_back(&env_vars, new_variable(ft_strdup("_"), ft_strdup("PATH")));
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
 	update_var(env_vars, "_", ft_strdup("PATH"));
	if (!get_env_key(env_vars ,"OLDPWD"))
		add_env_back(&env_vars, new_variable(ft_strdup("OLDPWD"), NULL)); // I add this, by default OLDPWD var is not included in envp
	return (env_vars);
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

int	ft_check_space_in_cmd(char *str)
{
	int	i;
	int	begining;
	int	len;

    if (!str)
        return (0);
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
		return (3);
	else if (begining == 1)
		return (2);
	else if (len != i)
		return (1);
	else
		return (0);
}


int	check_will_splited(t_env *envs, char **cmd, int i)
{
	int	j;
	int	check;
	int	start;
	char	*value;
	char *key;
	int nbr;

	key = NULL;
	value = NULL;
	nbr = 0;
	start = 0;
	check = 0;
	j = 0;
	if (i > 0)
	{
		if (cmd[i][0] != '"' && cmd[i][0] != '\'')
		{
			while (cmd[i][j])
			{
				if (cmd[i][j] == '$' && cmd[i][j + 1])
				{
					j++;
					nbr = 1;
					while (cmd[i][j] == '$' && cmd[i][j])
					{
						nbr++;
						j++;
					}
					if (nbr % 2 != 0)
					{
						start = j;
						while (cmd[i][j] && ft_isalpha(cmd[i][j]) && cmd[i][j] != '"')
							j++;
						key = ft_substr(cmd[i], start - 1, j - start);
						value = double_quotes_process(envs, key, NULL, 0);
						if (ft_check_space_in_cmd(value) > 0 || split_count_words(value, ' ') > 1)
							check = 1;
					}
					else
						check = 0;
				}
				else
					j++;
			}
			if (check == 1)
				return (1);
			
		}
		else if (cmd[i][0] == '"' || cmd[i][0] == '\'')
		{
			while (cmd[i - 1][j])
			{
				if (cmd[i - 1][j] == '$')
				{
					j++;
					nbr = 1;
					while (cmd[i - 1][j] == '$' && cmd[i - 1][j])
					{
						nbr++;
						j++;
					}
					if (nbr % 2 != 0)
					{
						start = j;
						while (cmd[i - 1][j] && ft_isalpha(cmd[i - 1][j]) && cmd[i - 1][j] != '"')
							j++;

						key = ft_substr(cmd[i - 1], start, j - start);
						value = get_env_variable(envs, key);
						if ((ft_check_space_in_cmd(value) > 0 || (split_count_words(value, ' ') > 1 && ft_check_space_in_cmd(cmd[i]) > 0 && ft_check_space_in_cmd(cmd[i - 1]) == 0)))
							check = 1;
					}
					else
						check = 0;
				}
				else
					j++;
			}
			if (check == 1)
				return (1);
		}
	}
	else
	{
		while (cmd[i][j])
		{
			if (cmd[i][j] == '$' && ft_isalpha(cmd[i][j + 1]))
				check = 1;
			j++;
		}
		if (check == 1)
			return (1);
	}
	return (free_str(key), free_str(value), 0);
}


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
t_vars ft_initialize_vars()
{
	t_vars vars;
	vars.i = 0;
	vars.index = 0;
	vars.j = 0;
	vars.tmp = NULL;
	vars.tmp1 = NULL;
	vars.new = NULL;
	vars.res = NULL;
	return (vars);
}