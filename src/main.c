/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 22:27:05 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/10 11:30:56 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	// t_env	*env_vars;
	
	char 	*tmp;
	char	**args;
	snprintf(prompt, sizeof(prompt), ANSI_COLOR_BOLD_GREEN "minishell $> " ANSI_COLOR_CYAN);
	// env_vars = expanding(envp);
	while ((input = readline(prompt)) != NULL)
	{
		tmp = input;
		add_history(tmp);
		remove_spaces(&input);
		if (check_syntax(input) == -1)
		{
			printf(ANSI_COLOR_RED "Syntax error\n");
			if (input)
				free(input);
			continue;
		}
		//printf("input : |%s|\n", input);
		input = add_spaces(input);
		args = split_args(input, ' ');
		cmds = split_cmds(args);
		// print_list(cmds);
		// execute(cmds, &env_vars); // I added this line
		printstrs(args);
		free_cmds(cmds);
		// printf("hna\n");
		free_args(args);
		if (tmp)
			free(tmp);
		free(input);
		leaks();
	}
	return 0;
}