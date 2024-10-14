/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 13:02:21 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/14 11:59:23 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_words(const char *s)
{
	int	i;
	int	nbr;
	int	single_quote;
	int	double_quote;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	nbr = 0;
	while (s[i])
	{
		ft_check_quotes(&single_quote, &double_quote, s[i]);
		while (s[i] && ft_isspace(s[i]) && !single_quote && !double_quote)
			i++;
		if (s[i])
			nbr++;
		while (!ft_isspace(s[i]) && !single_quote && !double_quote && s[i])
			i++;
	}
	return (nbr);
}

void	*ft_myfree(char **result, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (NULL);
}

char	**ft_allocate(int size)
{
	char	**result;

	result = (char **)malloc(sizeof(char *) * (size + 1));
	if (!result)
		exit(1);
	return (result);
}

char	*ft_strndup(char **r, char *s, int start, int end)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!tmp)
		exit(1);
	while (start < end)
	{
		tmp[i] = s[start];
		i++;
		start++;
	}
	tmp[i] = '\0';
	return (tmp);
}

char	**split_args(char *s)
{
	t_vars	vars;

	if (!s)
		return (NULL);
	vars = ft_initialize_vars();
	vars.res = ft_allocate(count_words(s) + 1);
	while (s[vars.i])
	{
		ft_check_quotes(&vars.single_quote, &vars.double_quote, s[vars.i]);
		if (ft_isspace(s[vars.i]) && !vars.single_quote && !vars.double_quote)
		{
			if (vars.start < vars.i)
				vars.res[vars.index++]
					= ft_strndup(vars.res, s, vars.start, vars.i);
			while (s[vars.i] && ft_isspace(s[vars.i]))
				vars.i++;
			vars.start = vars.i;
		}
		else
			vars.i++;
	}
	if (vars.start < vars.i)
		vars.res[vars.index++] = ft_strndup(vars.res, s, vars.start, vars.i);
	vars.res[vars.index] = NULL;
	return (free(s), vars.res);
}
