/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 09:30:36 by alamini           #+#    #+#             */
/*   Updated: 2024/10/13 10:06:16 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_exec.h"

void	reset_fd(t_path *path)
{
	dup2(path->fd_in, STDIN_FILENO);
	dup2(path->fd_out, STDOUT_FILENO);
}

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ex_strcmp("cd", cmd) == 0
		|| ex_strcmp("pwd", cmd) == 0
		|| ex_strcmp("echo", cmd) == 0
		|| ex_strcmp("echo", cmd) == 0
		|| ex_strcmp("export", cmd) == 0
		|| ex_strcmp("env", cmd) == 0
		|| ex_strcmp("unset", cmd) == 0
		|| ex_strcmp("exit", cmd) == 0)
		return (1);
	return (0);
}

int	ambigous_case(t_command *command, t_path *path, int i)
{
	int	pos;

	pos = -1;
	if (command->is_ambiguous != -1)
		pos = command->is_ambiguous - ft_strslen(command->cmd);
	if (pos == i + 1)
	{
		print_error(command->ambiguous_file, NULL, "ambiguous redirect");
		if (path->is_forked)
			exit(1);
		else
			return (1);
	}
	return (0);
}
