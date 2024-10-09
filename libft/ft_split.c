/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 17:25:03 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/09 22:24:47 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	split_count_words(const char *s, char charset)
{
	int	i;
	int	nbr;

	if (!s)
		return (0);
	i = 0;
	nbr = 0;
	while (s[i])
	{
		while (s[i] && ft_isspace(s[i]))
			i++;
		if (s[i] != '\0')
			nbr++;
		while (s[i] && !ft_isspace(s[i]))
			i++;
	}
	return (nbr);
}

static char	*ft_mystrdup( const char *s, char charset)
{
	char	*r;
	int		i;
	int		lw;

	charset = ' ';
	i = 0;
	lw = 0;
	while (s[lw] && !ft_isspace(s[lw]))
		lw++;
	r = (char *)malloc(lw + 1);
	if (r == NULL)
		return (NULL);
	while (i < lw)
	{
		r[i] = s[i];
		i++;
	}
	r[i] = '\0';
	return (r);
}

static void	*ft_myfree(char **result, int index)
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

static char	**ft_allocate(int size)
{
	char	**result;

	result = (char **)malloc(sizeof(char *) * (size + 1));
	if (!result)
		return (NULL);
	return (result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		index;
	char	*tmp;

	if (!s)
		return (NULL);
	index = 0;
	result = ft_allocate(split_count_words(s, c));
	while (*s)
	{
		while (*s && ft_isspace(*s))
			s++;
		if (*s != '\0')
		{
			tmp = ft_mystrdup(s, c);
			if (!tmp)
				return (ft_myfree(result, index));
			else
				result[index++] = tmp;
		}
		while (*s && !ft_isspace(*s))
			s++;
	}
	result[index] = 0;
	return (result);
}
