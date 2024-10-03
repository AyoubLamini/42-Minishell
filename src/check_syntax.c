/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 08:33:12 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/03 15:36:29 by ybouyzem         ###   ########.fr       */
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
	res[end+1] = '\0';
	*input = res;
}

int	check_directions(char *input)
{
	int	i;
	int	j;
	int	single_quote;
	int	double_quote;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	if (input[i] == '|')
		return (-1);
	while (input[i])
	{
		j = 0;
	
		ft_check_quotes(&single_quote, &double_quote, input[i]);
		if (!single_quote && !double_quote)
		{
			while (input[i] == '<' && input[i])
			{
				j++;
				i++;
			}
			if (j > 2)
				return (-1);
			j = 0;
			while (input[i] == '>' && input[i])
			{
				j++;
				i++;
			}
			if (input[i] != '\0' && input[i] != '<' && input[i] != '>')
				i++;
			if (j > 2)
				return (-2);
		}
		else
			i++;
	}
	return (1);
}

int	skip_spaces(char *str, int index)
{
	while (ft_isspace(str[index]) && str[index])
		index++;
	return (index);
}

int check_syntax(char *input)
{
	int i;
	int	j;
	int	single_quote;
	int	double_quote;

	single_quote = 0;
	double_quote = 0;
	i = 0;
	// printf("input: |%s|\n", input);
	if (check_directions(input) == -1)
		return (-1);
	 if (check_directions(input) == -2)
		return (-2);
	if (check_quotes(input) == -1)
		return (-1);
	
	// printf("here\n");
	if ((is_redirection(input, 0) == 2 && input[i + 2] == '\0') || (is_redirection(input, 0) && input[i + 1] == '\0'))
		return (-2);
	while (input[i])
	{
		j = i;
		ft_check_quotes(&single_quote, &double_quote, input[i]);
		if (!single_quote && !double_quote)
		{
			if (is_redirection(input, i) == 2)
			{
				i = skip_spaces(input, i + 1);
				if (input[i] == '|')
					return (-1);
			}
			if (input[i] == '>')
			{
				i = skip_spaces(input, i + 1);
				if ( input[i] == '\0')
					return (-1);
				if (j + 1 != i && input[i] == '>')
					return (-2);
				else if ( input[i] == '<' || input[i] == '<')
					return (-3);
				else if (input[i] == '|')
					return (-4);
			}
			if (input[i] == '<')
			{
				i = skip_spaces(input, i + 1);
				if (input[i] == '|')
					return (-4);
				else if (input[i] == '\0')
					return (-1);
			}
			if (input[i] == '|')
			{
				i = skip_spaces(input, i + 1);
				if (input[i] == '|' || input[i] == '\0')
					return (-4);
			}
		}
		i = j;
		i++;
	}
	return (1);
}