/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/14 15:29:13 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/14 15:29:43 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"

int	get_last_quote_pos(char	*old_cmd)
{
	int	len;

	len = ft_strlen(old_cmd);
	while (len && old_cmd[len] != '"')
		len--;
	return (len);
}

char	*single_quotes_process(char *str)
{
	int		i;
	char	*res;
	
	res = NULL;
	i = 0;
	str++;
	while (str[i] && str[i] != '\'')
		i++;
	if (str[i] == '\'')
		str[i] = '\0';
	res = ft_strjoin(res, str);
	return (res);
}


char	**single_quotes(t_env *envs, char **cmd, int i, char **res, int *index)
{
	char	*tmp;
	char	**temp;

	temp = NULL;
	tmp = NULL;
	tmp = single_quotes_process(cmd[i]);
	*index = ft_strslen(res);
	if (tmp && tmp[0] != '\0' && check_will_splited(envs, cmd, i) == 1 )
	{
		temp = ft_split(tmp, ' ');
		res = join_two_double_strs(res, temp);
	}
	else
	{	
		if (!res)
			res = join_double_strs_with_str(res, tmp);
		else
		{
			res[*index - 1] = ft_strjoin(res[*index - 1], tmp);
			res[*index] = 0;
		}
	}
	return (res);
}