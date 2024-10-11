#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"

int g_last_signal = 0;

void leaks() // TEMporary comment
{
	system("leaks minishell");
}
static void tty_attributes(struct termios *attrs, int action)
{
	if (action == ATTR_GET)
	{
		tcgetattr(STDIN_FILENO, &attrs[0]);
		tcgetattr(STDOUT_FILENO, &attrs[1]);
		tcgetattr(STDERR_FILENO, &attrs[2]);
	}
	else if (action == ATTR_SET)
	{
		tcsetattr(STDIN_FILENO, TCSANOW, &attrs[0]);
		tcsetattr(STDOUT_FILENO, TCSANOW, &attrs[1]);
		tcsetattr(STDERR_FILENO, TCSANOW, &attrs[2]);
	}
	else if (action == ATTR_CHG)
	{
		attrs[0].c_lflag &= ~ECHOCTL;
		attrs[1].c_lflag &= ~ECHOCTL;
		attrs[2].c_lflag &= ~ECHOCTL;
		tty_attributes(attrs, ATTR_SET);
	}
}
void	free_str(char *str)
{
	if (str)
		free(str);
}

static t_path *init_data(t_path *path)
{
	path = (t_path *)malloc(sizeof(t_path));
	path->exit_status = 0;
	path->is_forked = 0;
	path->fd_in = 0;
	path->fd_out = 1;
	path->heredoc = NULL;
	return (path);
}
void set_up(struct termios *attrs, t_path *path)
{
	setup_signals(path, SET_SIG); 
	tty_attributes(attrs, ATTR_GET); // Save terminal attributes
	tty_attributes(attrs, ATTR_CHG); // Change terminal attributes
}
int	main(int argc, char **argv, char **envp) // added envp argument
{
	struct termios	attrs[3];
	// if (!isatty(0))
	// {
	// 	printf("erorr\n");
	// 	exit(1);
	// }
	// printstrs(envp);
	(void)argc; 
	(void)argv;
	char	*input;
	input = NULL;
	char	prompt[100];
	t_command *cmds;
	t_env	*env_vars;
	t_path 	*path;
	char 	*tmp;
	char	**args;
	args = NULL;
	path = NULL;
	snprintf(prompt, sizeof(prompt),  "minishell $> "  ) ;
	env_vars = full_envs(envp);
	// print_envs(env_vars);
	path = init_data(path); // I added this line
	set_up(attrs, path); 
	path->exit_status = 0;
	while ((input = readline(prompt)) != NULL)
	{
		path->is_forked = 0;
		tmp = input;
		remove_spaces(&input);
		add_history(tmp);
		if (check_syntax(input) < 0)
		{
			// printf("returned value: %d\n", check_syntax(input));
			syntax_error_messages(check_syntax(input));
			exit_status(258, path);
			// printf(ANSI_COLOR_RED "Syntax error\n");
			continue;
		}
		input = add_spaces(input);
		input = ft_strtrim(input, " ");
		//printf("input : |%s|\n", input);
		args = split_args(input, ' ');
		//printstrs(args);
		//printf ("exit : %d\n", path.exit_status);
		cmds = split_cmds(args, env_vars, path);
		// print_list(cmds);
		execute(cmds, &env_vars, path); // I added this line
//		free_strs(args);
		if (tmp)
			free(tmp);
		free(input);
		// leaks();
		// process_heredocs(path);
		clear_herdocs(path);
		tty_attributes(attrs, ATTR_SET); // Reset terminal attributes
	}
	exit_status(path->exit_status, path);
	if (input)
		free(input);
	free_cmds(cmds);
	free_strs(args);
	free_envs(env_vars);
	// atexit(leaks);
	exit(path->exit_status);
}