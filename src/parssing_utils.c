/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/15 09:07:24 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/07/30 11:39:39 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int ft_isspace(char c)
{
	if (c == ' ' || (c >= 9 && c <= 13))
		return (1);
	return (0);
}

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

char	*add_spaces_helper(char *input, char *new, int single_quote, int double_quote)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (input[i])
	{
		ft_check_quotes(&single_quote, &double_quote, input[i]);
		if (!single_quote && !double_quote)
		{
			if (input[i + 1] && is_redirection(input, i) == 2)
			{
				new[j++] = ' ';
				new[j++] = input[i];
				new[j++] = input[i + 1];
				new[j] = ' ';
				i++;
			}
			else if (input[i] == '|' || input[i] == '<' || input[i] == '>')
			{
				new[j++] = ' ';
				new[j++] = input[i];
				new[j] = ' ';
			}
			else
				new[j] = input[i];
		}
		else
			new[j] = input[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
}
char    *add_spaces(char *input)
{
	char	*new;
	int		single_quote;
	int		double_quote;

	single_quote = 0;
	double_quote = 0;
	new = (char *)malloc(((ft_strlen(input) * 2) + 1) * sizeof(char));
	if (!new)
		return (free(input), NULL);
	new = add_spaces_helper(input, new, 0, 0);
	return (new);
}