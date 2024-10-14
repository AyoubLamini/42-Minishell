#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"

int g_last_signal = 0;

void leaks() // TEMporary comment
{
	system("leaks minishell");
}
static void tty_attributes(struct termios *attrs, int action)
{
	// if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO) || !isatty(STDERR_FILENO))
	// {
	// 	printf("Not a tty\n"),
	// 	exit(1);
	// }
		
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


static t_path *init_data(t_path *path)
{
	path = (t_path *)malloc(sizeof(t_path));
	path->exit_status = 0;
	path->is_forked = 0;
	path->fd_in = 0;
	path->fd_out = 1;
	path->heredoc = NULL;
	path->main_path = malloc(sizeof(char) * PATH_MAX);
	path->main_path = getcwd(path->main_path, PATH_MAX);
	path->pwd = my_malloc(sizeof(char) * PATH_MAX, 1);
	path->pwd = getcwd(path->pwd, PATH_MAX);
	
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
	//atexit(leaks);
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
	env_vars = NULL;
	cmds = NULL;	
	args = NULL;
	path = NULL;
	int exit_s;
	exit_s = 0;
	env_vars = full_envs(envp);
	// print_envs(env_vars);
	path = init_data(path); // I added this line
	set_up(attrs, path); 
	int i = 0;
	while ((input = readline("minishell $> ")) != NULL)
	{
		cmds = NULL;
		args = NULL;
		i++;
		path->is_forked = 0;
		//tmp = input;
		// if (i == 2)
		// 	exit(0);
		add_history(input);
		if (is_only_spaces(input))
		{
			free_str(input);
			continue;
		}
		input = ft_strtrim(input, " ");
		if (check_syntax(input) < 0)
		{
			syntax_error_messages(check_syntax(input));
			exit_status(258, path);
			free_str(input);
			continue;
		}
		input = add_spaces(input, 0, 0);
		input = ft_strtrim(input, " ");
		args = split_args(input);
		cmds = split_cmds(args, env_vars, path);
		print_list(cmds);
		execute(cmds, &env_vars, path); // I added this line
		//free_strs(args);
		// process_heredocs(path);
		//clear_herdocs(path);
		// my_malloc(0, 0);
		tty_attributes(attrs, ATTR_SET); // Reset terminal attributes
	}
	exit_s = path->exit_status;
	// free_str(input);
	// free_envs(env_vars);
	// free(path->main_path);	
	// free(path);
	exit(exit_s);
}
