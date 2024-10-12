/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:07:24 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/12 06:24:23 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int	is_redirection(char *str, int index)
{
	if (str[index] == '>' && str[index + 1] == '>')
		return (2);
	if (str[index] == '<' && str[index + 1] == '<')
		return (2);
	if (str[index] == '<' || str[index] == '>')
		return (1);
	return (0);
}

void	ft_check_quotes(int *single_quote, int *double_quote, char c)
{
	if (c == '\'' && !*single_quote)
		*double_quote = !*double_quote;
	else if (c == '"' && !*double_quote)
		*single_quote = !*single_quote;
}

void	add_spaces_helper(char **new, char *input, int *i, int *j, int type)
{
	if (type == 0)
		(*new)[*j] = input[*i];
	if (type == 1)
	{
		(*new)[*j] = ' ';
		(*j) += 1;
		(*new)[*j] = input[*i];
		(*j) += 1;
		(*new)[*j] = ' ';
		
	}
	else if (type == 2)
	{
		(*new)[*j] = ' ';
		(*j) += 1;
		(*new)[*j] = input[*i];
		(*j) += 1;
		(*new)[*j] = input[*i + 1];
		(*j) += 1;
		(*new)[*j] = ' ';
		(*i)++;
	}
	(*i)++;
	(*j)++;
}

char    *add_spaces(char *input, int single_quote, int double_quote)
{	
	t_vars	vars;

	vars = ft_initialize_vars();
	vars.new = (char *)malloc(((ft_strlen(input) * 2) + 1) * sizeof(char));
	if (!vars.new)
		free(input), exit(1);
	while (input[vars.i])
	{
		ft_check_quotes(&single_quote, &double_quote, input[vars.i]);
		if (!single_quote && !double_quote)
		{
			if (input[vars.i + 1] && is_redirection(input, vars.i) == 2)
				add_spaces_helper(&vars.new, input, &vars.i, &vars.j, 2);
			else if (input[vars.i] == '|' || input[vars.i] == '<' || input[vars.i] == '>')
				add_spaces_helper(&vars.new, input, &vars.i, &vars.j, 1);
			else
				add_spaces_helper(&vars.new, input, &vars.i, &vars.j, 0);
		}
		else
			add_spaces_helper(&vars.new, input, &vars.i, &vars.j, 0);
	}
	vars.new[vars.j] = '\0';
	return (free(input), vars.new);
}
