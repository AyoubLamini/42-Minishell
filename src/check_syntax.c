/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:33:12 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/13 01:04:05 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_quotes(char	*input)
{
	int		i;
	int		single_quote;
	int		double_quote;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	while (input[i])
	{
		if (input[i] == '\'' && !single_quote)
			double_quote = !double_quote;
		else if (input[i] == '"' && !double_quote)
			single_quote = !single_quote;
		i++;
	}
	if (single_quote || double_quote)
		return (-1);
	return (1);
}

void	remove_spaces(char **input)
{
	char	*res;
	int		start;
	int		end;
	int		i;

	res = *input;
	start = 0;
	i = 0;
	if (!res)
		return ;
	while (ft_isspace(*res))
		res++;
	end = ft_strlen(res) - 1;
	while (end >= start && ft_isspace(res[end]))
		end--;
	res[end + 1] = '\0';
	*input = res;
}

int	check_redirections_helper(char *input, int *i, int *j, char c)
{
	if (c == '>')
	{
		while (input[*i] == '>' && input[*i])
		{
			*j = *j + 1;
			*i = *i + 1;
		}
		if (*j > 2)
			return (-2);
	}
	else if (c == '<')
	{
		while (input[*i] == '<' && input[*i])
		{
			*j = *j + 1;
			*i = *i + 1;
		}
		if (*j > 2)
			return (-1);
	}
	return (0);
}

int	check_directions(char *input)
{
	t_vars	vars;

	vars = ft_initialize_vars();
	if (input[vars.i] == '|')
		return (-1);
	while (input[vars.i])
	{
		vars.j = 0;
		ft_check_quotes(&vars.single_quote, &vars.double_quote, input[vars.i]);
		if (!vars.single_quote && !vars.double_quote)
		{
			if (check_redirections_helper(input, &vars.i, &vars.j, '<') == -1)
				return (-1);
			vars.j = 0;
			if (check_redirections_helper(input, &vars.i, &vars.j, '>') == -2)
				return (-2);
			if (input[vars.i] != '\0' && input[vars.i] != '<'
				&& input[vars.i] != '>')
				vars.i++;
		}
		else
			vars.i++;
	}
	return (1);
}

int	skip_spaces(char *str, int index, int *j)
{
	*j = index;
	while (ft_isspace(str[index]) && str[index])
		index++;
	return (index);
}

int	check_syntax(char *input)
{
	t_vars	vars;
	
	vars = ft_initialize_vars();
	if (check_directions(input) == -1 || check_quotes(input) == -1)
		return (-1);
	if (check_directions(input) == -2)
		return (-2);
	if ((is_redirection(input, 0) == 2 && input[vars.i + 2] == '\0') || (is_redirection(input, 0) && input[vars.i + 1] == '\0'))
		return (-2);
	while (input[vars.i])
	{
		vars.j = vars.i;
		ft_check_quotes(&vars.single_quote, &vars.double_quote, input[vars.i]);
		if (!vars.single_quote && !vars.double_quote)
		{
			if (is_redirection(input, vars.i) == 2)
			{
				vars.i = skip_spaces(input, vars.i + 1, &vars.j);
				if (input[vars.i] == '|')
					return (-1);
			}
			if (input[vars.i] == '>')
			{
				vars.i = skip_spaces(input, vars.i + 1, &vars.j);
				if ( input[vars.i] == '\0')
					return (-1);
				if (vars.j + 1 != vars.i && input[vars.i] == '>')
					return (-2);
				else if ( input[vars.i] == '<')
					return (-3);
				else if (input[vars.i] == '|')
					return (-4);
			}
			if (input[vars.i] == '<')
			{
				vars.i = skip_spaces(input, vars.i + 1, &vars.j);
				if (input[vars.i] == '|')
					return (-4);
				else if (vars.i != vars.j && input[vars.i] == '<')
					return (-3);
				else if (input[vars.i] == '>')
					return (-2);
				else if (input[vars.i] == '\0')
					return (-1);
			}
			if (input[vars.i] == '|')
			{
				vars.i = skip_spaces(input, vars.i + 1, &vars.j);
				if (input[vars.i] == '|' || input[vars.i] == '\0')
					return (-4);
			}
		}
		vars.i = vars.j;
		vars.i++;
	}
	return (1);
}
