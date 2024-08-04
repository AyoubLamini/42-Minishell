/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 22:27:05 by ybouyzem          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/04 13:12:29 by alamini          ###   ########.fr       */
=======
/*   Updated: 2024/08/03 13:56:24 by ybouyzem         ###   ########.fr       */
>>>>>>> 2d7fb986fad5d9bfa6da38043eea70b1c9ab5838
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


void leaks()
{
	system("leaks minishell");
}



int	main(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	char	*input;
	char	prompt[100];
	t_command *cmds;
	//t_env	*env_vars;
	
	char 	*tmp;
	char	**args;
	snprintf(prompt, sizeof(prompt), ANSI_COLOR_BOLD_GREEN "minishell $> " ANSI_COLOR_CYAN);
	while ((input = readline(prompt)) != NULL)
	{
		tmp = input;
		add_history(tmp);
		remove_spaces(&input);
		if (check_syntax(input) == -1)
		{
			printf(ANSI_COLOR_RED "Syntax error\n");
			continue;
		}
		//printf("input : |%s|\n", input);
		input = add_spaces(input);
		args = split_args(input, ' ');
		cmds = split_cmds(args);
<<<<<<< HEAD
		execute(cmds); // added this line only
		env_vars = expanding(envp);
		args = ft_myfree(args, ft_strslen(args));
=======
		//env_vars = expanding(envp);
		// print_list(cmds);
		free_cmds(&cmds);
		printf("hna\n");
		//free_strs(args);
		if (tmp)
			free(tmp);
>>>>>>> 2d7fb986fad5d9bfa6da38043eea70b1c9ab5838
		free(input);
		leaks();
	}
	return 0;
}