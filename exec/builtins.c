/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:34:00 by alamini           #+#    #+#             */
/*   Updated: 2024/10/13 10:47:27 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_exec.h"

int	env(t_env *env_vars)
{
	while (env_vars)
	{
		if (get_env_value(env_vars, env_vars->key))
		{
			printf("%s=", env_vars->key);
			printf("%s\n", env_vars->value);
		}
		env_vars = env_vars->next;
	}
	return (0);
}

int	pwd(t_path *path) // pwd
{
	char	*pwd;

	pwd = path->pwd;
	if (pwd)
	{
		printf("%s\n", pwd);
		return (0);
	}
	return (1);
}

void	exit_shell(t_command *command, t_path *path)
{
	if (!command->cmd[1])
	{
		printf("exit\n");
		exit(path->exit_status);
	}
	if (!ft_is_numeric(command->cmd[1]))
	{
		if (!path->is_forked)
			ft_putstr_fd("exit\n", 1);
		print_error("exit", command->cmd[1], "numeric argument required");
		exit(255);
	}
	else if (command->cmd[2])
	{
		if (!path->is_forked)
			ft_putstr_fd("exit\n", 1);
		print_error("exit", NULL, "too many arguments");
		exit_status(1, path);
	}
	else
	{
		if (!path->is_forked)
			printf("exit\n");
		exit(ft_atoi(command->cmd[1]));
	}
}
