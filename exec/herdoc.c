/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 05:13:23 by alamini           #+#    #+#             */
/*   Updated: 2024/10/15 17:16:00 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_exec.h"

static char	*ft_rename(void)
{
	static int	id;
	char		*file;

	id++;
	file = my_strjoin(my_strdup("/tmp/.herdoc"), my_itoa(id));
	return (file);
}

t_heredoc	*init_herdoc(char *delimiter, t_path *path, t_env *env)
{
	t_heredoc	*heredoc;

	setup_signals(path, HERDOC_SIG);
	delimiter = get_right_delimeter(delimiter);
	heredoc = lst_heredoc_new(my_strdup(delimiter), ft_rename());
	if (!heredoc)
		malloc_error(path, env);
	heredoc->will_expand = check_will_expanded(delimiter);
	lst_heredoc_add_back(&path->heredoc, heredoc);
	path->fd_in = dup(STDIN_FILENO);
	return (heredoc);
}

int	read_loop(t_path *path, t_env **envs, t_heredoc *hdoc)
{
	char	*line;

	line = NULL;
	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			if (g_last_signal == SIGINT)
				return (g_last_signal = 0, exit_status(1, path), 1);
			break ;
		}
		if (ft_strcmp(line, hdoc->delimiter) == 0)
		{
			free(line);
			break ;
		}
		if (hdoc->will_expand)
			line = expanding_cmd_herdoc(*envs, line, *path);
		hdoc->buffer = my_strjoin(hdoc->buffer, line);
		hdoc->buffer = my_strjoin(hdoc->buffer, "\n");
	}
	return (0);
}

static int	heredoc(t_command *cmd, t_path *path, char *delimiter, t_env **envs)
{
	t_heredoc	*heredoc;
	int			fd;

	heredoc = init_herdoc(delimiter, path, *envs);
	if (read_loop(path, envs, heredoc))
		return (1);
	fd = open(heredoc->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_error("heredoc", NULL, strerror(errno));
		exit_status(1, path);
		exit(path->exit_status);
	}
	write(fd, heredoc->buffer, ex_strlen(heredoc->buffer));
	cmd->last_file = my_strdup(heredoc->file);
	close(fd);
	setup_signals(path, SET_SIG);
	return (0);
}

int	handle_herdoc(t_command *command, t_path *path, t_env **envs)
{
	int	i;

	i = 0;
	if (!is_any_heredoc(command->redirection))
		return (0);
	while (command->redirection[i])
	{
		if (ex_strcmp(command->redirection[i], "<<") == 0)
		{
			if (heredoc(command, path, command->redirection[i + 1], envs))
			{
				setup_signals(path, SET_SIG);
				reset_fd(path);
				close(path->fd_in);
				return (1);
			}
			else
			{
				close(path->fd_in);
				path->fd_in = 0;
			}
		}
		i++;
	}
	return (0);
}
