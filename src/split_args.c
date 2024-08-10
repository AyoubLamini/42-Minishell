/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 13:02:21 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/09 13:58:56 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int count_words(const char *s, char charset)
{
	int i;
	int nbr;
	int	single_quote;
	int	double_quote;
	i = 0;
	
	single_quote = 0;
	double_quote = 0;
	nbr = 0;
	while (s[i])
	{
		ft_check_quotes(&single_quote, &double_quote, s[i]);
		while (s[i] && s[i] == charset && !single_quote && !double_quote)
			i++;
		if (s[i])
			nbr++;
		while (s[i] != charset && !single_quote && !double_quote && s[i])
			i++;
	}
	return (nbr);
}

void *ft_myfree(char **result, int index)
{
	int i = 0;

	while (i < index)
	{
		free(result[i]);
		i++;
	}
	free(result);
	return (NULL);
}

static char **ft_allocate(int size)
{
	char **result;

	result = (char **)malloc(sizeof(char *) * (size + 1));
	if (!result)
		return (NULL);
	return (result);
}

void	ft_strndup(char **r, char	*s, int start, int end, int *index)
{
	char	*tmp;
	int		i;

	i = 0;
	tmp = (char *)malloc(sizeof(char) * (end - start + 1));
	if (!tmp)
		return ;
	while(start < end)
	{
		tmp[i] = s[start];
		i++;
		start++;
	}
	tmp[i] = '\0';
	if (!tmp)
	{
		ft_myfree(r, *index);
		return ;
	}
	r[*index] = tmp;
	*index += 1;
}

void	split_helper(char	**r, char *s,char token, int *index)
{
	int i;
	int start;
	int single_quote;
	int double_quote;

	i = 0;
	start = 0;
	single_quote = 0;
	double_quote = 0;
	while (s[i] && s[i] == token)
		i++;
	while (s[i])
	{
		ft_check_quotes(&single_quote, &double_quote, s[i]);
		if (s[i] == token && !single_quote && !double_quote)
		{
			if (start < i)
				ft_strndup(r, s, start, i, index);
			while (s[i] && s[i] == token)
				i++;
			start = i;
		}
		else
			i++;
	}
	if (start < i)
		ft_strndup(r, s, start, i , index);
	r[*index] = NULL;
}

char	**split_args(char *s, char c)
{
	char **result;
	int index;
	
	index = 0;
	if (!s)
		return NULL;
	result = ft_allocate(count_words(s, c)  + 1);
	if (!result)
		return NULL;
	split_helper(result, s, c, &index);
	return (result);
}