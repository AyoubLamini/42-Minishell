#include "../includes/minishell.h"


void leaks() // TEMporary comment
{
	system("leaks minishell");
}

int	main(int argc, char **argv, char **envp) // added envp argument
{
	(void)argc;
	(void)argv;
	(void)envp;
	char	*input;
	char	prompt[100];
	t_command *cmds;
	t_env	*env_vars;
	char 	*tmp;
	char	**args;
	snprintf(prompt, sizeof(prompt), ANSI_COLOR_BOLD_GREEN "minishell $> " ANSI_COLOR_BLUE ) ;
	env_vars = full_envs(envp);
	// print_envs(env_vars);
	while ((input = readline(prompt)) != NULL)
	{
		tmp = input;
		remove_spaces(&input);
		add_history(tmp);
		if (check_syntax(input) == -1)
		{
			printf(ANSI_COLOR_RED "Syntax error\n");
			if (input)
				free(input);
			continue;
		}
		input = add_spaces(input);
		args = split_args(input, ' ');
		cmds = split_cmds(args, env_vars);

		execute(cmds, &env_vars); // I added this line
		free_cmds(cmds);
		free_strs(args);
		if (tmp)
			free(tmp);
		free(input);
		// leaks();
	}
	free_envs(env_vars);
	// atexit(leaks);
	return 0;
}