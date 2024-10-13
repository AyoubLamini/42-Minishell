/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 05:46:27 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/13 03:47:54 by ybouyzem         ###   ########.fr       */
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
	t_vars	vars;

	vars = ft_initialize_vars();
	while (cmd[index][vars.i])
	{
		if (cmd[index][vars.i] == '$')
		{
			vars.i++;
			vars.start = 1;
			while (cmd[index][vars.i] && cmd[index][vars.i] == '$')
			{
				vars.start++;
				vars.i++;
			}
			if (cmd[index + 1] != NULL)
			{
				if (vars.start % 2 != 0 && cmd[index][vars.i] == '\0'
					&& (cmd[index + 1][0] == '"' || cmd[index + 1][0] == '\''))
					return (1);	
			}
		}
		else
			vars.i++;
	}
	return (0);
}

int	ft_check_space_in_cmd(char *str)
{
	int	begining;
	t_vars	vars;

	vars = ft_initialize_vars();
    if (!str)
        return (0);
	if (str[0] == '\0')
		return (0);
	begining = 0;
	while (str[vars.i] && ft_isspace(str[vars.i]))
		vars.i++;
	if (vars.i > 0)
		begining = 1;
	vars.len = ft_strlen(str) - 1;
	vars.i = vars.len;
	while (vars.i && ft_isspace(str[vars.i]))
		vars.i--;
	if (vars.len != vars.i && begining == 1)
		return (3);
	else if (begining == 1)
		return (2);
	else if (vars.len != vars.i)
		return (1);
	else
		return (0);
}

void	skip_dollars(char **cmd, int i, int *j, int *nbr)
{
	*j += 1;
	*nbr = 1;
	while (cmd[i][*j] == '$' && cmd[i][*j])
	{
		*nbr += 1;
		*j += 1;
	}
}

int	check_will_splited_helper1(t_env *envs, t_vars vars, char **cmd, int i)
{
	while (cmd[i][vars.j])
	{
		if (cmd[i][vars.j] == '$' && cmd[i][vars.j + 1])
		{
			vars.nbr = 1;
			skip_dollars(cmd, i, &vars.j, &vars.nbr);
			if (vars.nbr % 2 != 0)
			{
				vars.start = vars.j;
				while (cmd[i][vars.j] && ft_isalpha(cmd[i][vars.j]) && cmd[i][vars.j] != '"')
					vars.j++;
				vars.key = ft_substr(cmd[i], vars.start - 1, vars.j - vars.start);
				vars.value = double_quotes_process(envs, vars.key, NULL, 0);
				if (ft_check_space_in_cmd(vars.value) > 0 || split_count_words(vars.value, ' ') > 1)
					vars.check = 1;
			}
			else
				vars.check = 0;
		}
		else
			vars.j++;
	}
	return (vars.check);
}

int	check_will_splited_helper2(t_env *envs, t_vars vars, char **cmd, int i)
{
	while (cmd[i - 1][vars.j])
	{
		if (cmd[i - 1][vars.j] == '$')
		{
			skip_dollars(cmd, i - 1, &vars.j, &vars.nbr);
			if (vars.nbr % 2 != 0)
			{
				vars.start = vars.j;
				while (cmd[i - 1][vars.j] && ft_isalpha(cmd[i - 1][vars.j]) && cmd[i - 1][vars.j] != '"')
					vars.j++;
				vars.key = ft_substr(cmd[i - 1], vars.start, vars.j - vars.start);
				vars.value = get_env_variable(envs, vars.key);
				if ((ft_check_space_in_cmd(vars.value) > 0 || (split_count_words(vars.value, ' ') > 1 && ft_check_space_in_cmd(cmd[i]) > 0 && ft_check_space_in_cmd(cmd[i - 1]) == 0)))
					vars.check = 1;
			}
			else
				vars.check = 0;
		}
		else
			vars.j++;
	}
	return (vars.check);
}

int	check_will_splited_helper3(t_env *envs, t_vars vars, char **cmd, int i)
{
	while (cmd[i][vars.j])
	{
		if (cmd[i][vars.j] == '$' && ft_isalpha(cmd[i][vars.j + 1]))
			vars.check = 1;
		vars.j++;
	}
	if (vars.check == 1)
		return (1);
	return (0);
}

int	check_will_splited(t_env *envs, char **cmd, int i)
{
	t_vars	vars;

	vars = ft_initialize_vars();
	if (i > 0)
	{
		if (cmd[i][0] != '"' && cmd[i][0] != '\'')
		{
			if (check_will_splited_helper1(envs, vars, cmd, i))
				return (1);
		}
		else if (cmd[i][0] == '"' || cmd[i][0] == '\'')
		{
			if (check_will_splited_helper2(envs, vars, cmd, i))
				return (1);
		}
	}
	else
	{
		if (check_will_splited_helper3(envs, vars, cmd, i))
			return (1);
	}
	return (0);
}
