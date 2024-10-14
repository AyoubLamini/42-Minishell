/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/04 21:01:44 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/14 17:20:12 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	*free_str(char *str)
{
	if (str)
		free(str);
	return (NULL);
}

static size_t	ft_mysize(const char *start, const char *end)
{
	size_t	startlen;
	size_t	endlen;
	size_t	size;

	startlen = ft_strlen(start);
	endlen = ft_strlen(end);
	size = startlen - endlen;
	return (size);
}


char	*ft_strtrim(char *s1, char const *set)
{
	const char	*end;
	char		*r;
	size_t		i;
	size_t		j;

	if (!s1 || !set)
		return (NULL);
	i = 0;
	j = 0;
	end = s1 + ft_strlen(s1) - 1;
	while (s1[j] && ft_isspace(s1[j]))
		j++;
	while (end >= s1 && ft_isspace(*end))
		end--;
	end += 1;
	r = (char *)malloc(ft_mysize(s1, end) + 1);
	if (r == NULL)
		return (NULL);
	while (j < ft_mysize(s1, end))
	{
		r[i] = s1[j];
		i++;
		j++;
	}
	r[i] = '\0';
	return (free_str(s1), r);
}
