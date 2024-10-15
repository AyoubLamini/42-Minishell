/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 10:08:19 by alamini           #+#    #+#             */
/*   Updated: 2024/10/15 17:20:12 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_exec.h"

size_t	ex_strlen(const char *s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}

char	*ex_strdup(const char *s1)
{
	char	*ptr;
	int		size;
	int		i;

	size = ex_strlen(s1);
	ptr = malloc((sizeof(char) * (size + 1)));
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < size)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*my_strdup(const char *s1)
{
	char	*ptr;
	int		size;
	int		i;

	size = ex_strlen(s1);
	ptr = my_malloc((sizeof(char) * (size + 1)), 1);
	if (!ptr)
		return (NULL);
	i = 0;
	while (i < size)
	{
		ptr[i] = s1[i];
		i++;
	}
	ptr[i] = '\0';
	return (ptr);
}

char	*ex_strjoin(char *s1, char *s2)
{
	char	*res;
	int		length;
	int		i;
	int		j;

	if (!s1)
		return (ex_strdup(s2));
	if (!s2)
		return (s1);
	length = ex_strlen(s1) + ex_strlen(s2);
	res = (char *)malloc(sizeof(char) * length + 1);
	if (!res)
		return (0);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	return (free(s1), res);
}

char	*my_strjoin(char *s1, char *s2)
{
	char	*res;
	int		length;
	int		i;
	int		j;

	if (!s1)
		return (my_strdup(s2));
	if (!s2)
		return (s1);
	length = ex_strlen(s1) + ex_strlen(s2);
	res = (char *)my_malloc(sizeof(char) * length + 1, 1);
	if (!res)
		return (NULL);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	j = 0;
	while (s2[j])
		res[i++] = s2[j++];
	res[i] = '\0';
	return (res);
}

static int	ft_size(long n)
{
	int	size;

	if (n == 0)
		return (1);
	if (n < 0)
	{
		n *= -1;
		size = 1;
	}
	else
		size = 0;
	while (n != 0)
	{
		size++;
		n /= 10;
	}
	return (size);
}

char	*my_itoa(int n)
{
	char	*str;
	int		size;

	size = ft_size(n);
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	str = (char *)my_malloc(size + 1, 1);
	if (str == NULL)
		return (NULL);
	str[size] = '\0';
	size -= 1;
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	if (n == 0)
		str[size] = 48;
	while (n > 0)
	{
		str[size] = n % 10 + '0';
		n /= 10;
		size--;
	}
	return (str);
}
