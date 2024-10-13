/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   piping.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 08:41:32 by alamini           #+#    #+#             */
/*   Updated: 2024/10/13 09:21:26 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_exec.h"

static int	create_pipe(int *fd)
{
	if (pipe(fd) < 0)
	{
		printf("Error :Pipe No created !!");
		exit(1);
	}
	return (0);
}

static int	child_process(t_command *cmd, t_env **env, int *input_fd,
	t_path *path)
{
	setup_signals(path, UNSET_SIG);
	if (*input_fd != -1)
	{
		if (dup2(*input_fd, STDIN_FILENO) < 0)
		{
			perror("dup2");
			return (1);
		}
		close(*input_fd);
	}
	if (cmd->next)
	{
		if (dup2(cmd->pipe[1], STDOUT_FILENO) < 0)
			return (perror("dup2"), 1);
		close(cmd->pipe[0]);
		close(cmd->pipe[1]);
	}
	if (cmd->redirection[0])
		handle_redirection(cmd, path);
	check_command(cmd, env, path);
	exit(0);
}

static int	parent_process(t_command *cmd, int *input_fd)
{
	if (*input_fd != -1)
		close(*input_fd);
	if (cmd->next)
	{
		close(cmd->pipe[1]);
		*input_fd = cmd->pipe[0];
	}
	else
		close(cmd->pipe[0]);
	return (0);
}

void	piping(t_command *cmd, t_env **env, int *input_fd, t_path *path)
{
	int	pid;

	if (cmd->next)
		create_pipe(cmd->pipe);
	pid = fork();
	path->is_forked = 1;
	if (pid < 0)
		return (perror("fork"));
	if (pid == 0)
	{
		if (child_process(cmd, env, input_fd, path))
			return ;
	}
	else
	{
		if (parent_process(cmd, input_fd))
			return ;
	}
	return ;
}
