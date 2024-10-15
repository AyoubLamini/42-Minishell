#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"

int g_last_signal = 0;

void leaks() // TEMporary comment
{
	system("leaks minishell");
}
static void tty_attributes(struct termios *attrs, int action)
{
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO) || !isatty(STDERR_FILENO))
	{
		printf("Not a tty\n"),
		exit(1);
	}
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
	if (!path)
		ex_malloc_error();
	path->exit_status = 0;
	path->is_forked = 0;
	path->fd_in = 0;
	path->fd_out = 1;
	path->heredoc = NULL;
	path->main_path = malloc(sizeof(char) * PATH_MAX);
	if (!path->main_path)
		ex_malloc_error();
	path->main_path = getcwd(path->main_path, PATH_MAX);
	path->pwd = malloc(sizeof(char) * PATH_MAX);
	if (!path->pwd)
		ex_malloc_error();
	path->pwd = getcwd(path->pwd, PATH_MAX);
	return (path);
}

void free_and_exit(t_path *path, t_env *env_vars)
{
	int exit_state;

	exit_state = path->exit_status;
	free_envs(env_vars);
	free(path->pwd);
	free(path->main_path);	
	free(path);
	exit(exit_state);
}
void set_up(struct termios *attrs, t_path *path)
{
	setup_signals(path, SET_SIG); 
	tty_attributes(attrs, ATTR_GET); // Save terminal attributes
	tty_attributes(attrs, ATTR_CHG); // Change terminal attributes
}
int	main(int argc, char **argv, char **envp) // added envp argument
{
		// atexit(leaks);
	//atexit(leaks);
	struct termios	attrs[3];
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
	env_vars = full_envs(envp);
	// print_envs(env_vars);
	path = init_data(path); // I added this line
	set_up(attrs, path); 
	while ((input = readline("minishell $> ")) != NULL)
	{
		path->is_forked = 0;
		add_history(input);
		if (is_only_spaces(input))
		{
			free(input);
			continue;
		}
		tmp = ft_strtrim(input, " ");
		free(input);
		if (check_syntax(tmp) < 0)
		{
			syntax_error_messages(check_syntax(tmp));
			exit_status(258, path);
			
			continue;
		}
		tmp = add_spaces(tmp, 0, 0);
		tmp = ft_strtrim(tmp, " ");
		args = split_args(tmp);
		cmds = split_cmds(args, env_vars, path);
		//print_list(cmds);
		execute(cmds, &env_vars, path); // I added this line
		clear_herdocs(path);
		my_malloc(0, 0);
		path->heredoc = NULL;
		tty_attributes(attrs, ATTR_SET); // Reset terminal attributes
	}
	free_and_exit(path, env_vars);
}
