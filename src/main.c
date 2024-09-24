#include "../includes/minishell.h"


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
	char	prompt[100];
	t_command *cmds;
	t_env	*env_vars;
	t_path path;
	char 	*tmp;
	char	**args;
	args = NULL;
	snprintf(prompt, sizeof(prompt),  "minishell $> "  ) ;
	env_vars = full_envs(envp);
	// print_envs(env_vars);
	set_up(attrs, &path); 
	path.exit_status = 0;
	while ((input = readline(prompt)) != NULL)
	{
		tmp = input;
		remove_spaces(&input);
		add_history(tmp);
		if (check_syntax(input) < 0)
		{
			// printf("returned value: %d\n", check_syntax(input));
			syntax_error_messages(check_syntax(input));
			// printf(ANSI_COLOR_RED "Syntax error\n");
			if (input)
				free(input);
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
		execute(cmds, &env_vars, &path); // I added this line
		free_cmds(cmds);
//		free_strs(args);
		if (tmp)
			free(tmp);
		free(input);
		// leaks();
		tty_attributes(attrs, ATTR_SET); // Reset terminal attributes
	}
	free_envs(env_vars);
	// atexit(leaks);
	return 0;
}