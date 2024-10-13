/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 05:05:44 by alamini           #+#    #+#             */
/*   Updated: 2024/10/13 07:38:34 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_exec.h"

void	exit_status(int status, t_path *path)
{
	path->exit_status = status;
	return ;
}

void	set_last_arg(t_command *command, t_env **env_vars)
{
	t_command	*current;
	int			i;

	i = 0;
	current = command;
	if (!current || !current->cmd[0])
		return ;
	while (current->cmd[i])
		i++;
	if (!current->cmd[i] && i == 1
		&& ex_strcmp(current->cmd[i - 1], "echo") == 0)
	{
		update_var(*env_vars, "_", ft_strdup("\0"));
	}
	else
		update_var(*env_vars, "_", ft_strdup(current->cmd[i - 1]));
}

int	is_any_heredoc(char **red)
{
	int	i;

	i = 0;
	if (!red || !red[0])
		return (0);
	while (red[i])
	{
		if (ex_strcmp(red[i], "<<") == 0)
			return (1);
		i++;
	}
	return (0);
}
