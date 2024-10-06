/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_herdoc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/03 09:59:57 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/06 22:17:32 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expanding_herdoc(t_env *envs, char *str, t_path path)
{
	int		i;
	char	*res;
	int		start;
	char	*key;

	key = NULL;
	res= NULL;
	start = 0;
	i = 0;
	if (str[0] == '"' || str[0] == '\'')
		str++;
	while (str[i] && str[i] != '"' && str[i] != '\'')
	{
		start = i;
		while (str[i] && str[i] != '$' && str[i] != '"')
			i++;
		key = ft_substr(str, start, i - start);
		res = ft_strjoin(res, key);
		if (str[i] == '$')
		{
			i++;
			start = 1;
			while (str[i] == '$' && str[i])
			{
				start++;
				i++;
			}
			if (start % 2 == 0)
			{
				start = start / 2;
				while (start)
				{
					res = ft_strjoin(res, "$");
					start--;
				}
			}
			else
			{
				start = start / 2;
				while (start >= 1)
				{
					res = ft_strjoin(res, "$");
					start--;
				}
				if (str[i] == '?')
				{
					res = ft_strjoin(res, ft_itoa(path.exit_status));
					i++;
				}
				else if (ft_isdigit(str[i]))
					i++;
				else
				{
					if (!ft_isalpha(str[i]))
						res = ft_strjoin(res, "$");
					start = i;
					while (ft_isalnum(str[i]) && str[i])
						i++;
					key = ft_substr(str, start, i - start);
					if (get_env_variable(envs, key))
						res = ft_strjoin(res, get_env_variable(envs, key));
				}
			}
		}
	}
	res = ft_strjoin(res, "\0");
	return (res);
}

char    *expanding_cmd_herdoc(t_env *envs, char *old_cmd, t_path path)
{
    char    **cmd;
    char    *tmp;
    char    *res;
	int    i;
    
	tmp = NULL;
	res = NULL;
    i = 0;
    cmd = expanding_split(old_cmd);
    while (cmd[i])
    {
        if (cmd[i][0] == '\'')
        {
            res = ft_strjoin(res, "'");
            tmp = expanding_herdoc(envs, cmd[i], path);
            res = ft_strjoin(res, tmp);
            if (cmd[i][ft_strlen(cmd[i]) - 1] == '\'')
                res = ft_strjoin(res, "'");
        }
        else if (cmd[i][0] == '"')
        {	
            res = ft_strjoin(res, "\"");
            tmp = expanding_herdoc(envs, cmd[i], path);
            res = ft_strjoin(res, tmp);
            if (cmd[i][ft_strlen(cmd[i]) - 1] == '"')
                res = ft_strjoin(res, "\"");
        }
        else
        {
            tmp = expanding_herdoc(envs, cmd[i], path);
            res = ft_strjoin(res, tmp);
        }
        i++;
    }
    return (free_strs(cmd), res);
}

char	*get_right_delimeter(char *s)
{
	int		i;
	char	*delimeter;
	char	**del;
	int		j;
	
	delimeter = NULL;
	del = NULL;
	i = 0;
	del = expanding_split(s);
	while (del[i])
	{
		j = 0;
		if (del[i][0] == '\'')
		{
			j++;
			while (del[i][j] && del[i][j] != '\'')
				j++;
			delimeter = ft_strjoin(delimeter, ft_substr(del[i], 1, j - 1));
		}
		else if (del[i][0] == '"')
		{
			j++;
			while (del[i][j] && del[i][j] != '"')
				j++;
			delimeter = ft_strjoin(delimeter, ft_substr(del[i], 1, j - 1));
		}
		else if (del[i][0] == '$' && del[i][1] == '\0' && (del[i + 1][0] == '"' || del[i + 1][0] == '\''))
		{
			
		}
		else
			delimeter = ft_strjoin(delimeter, del[i]);
		i++;
	}
	return (delimeter);
}

int	check_will_expanded(char *delimter)
{
	int i;

	i = 0;
	while (delimter[i])
	{
		if (delimter[i] == '"' || delimter[i] == '\'')
			return (0);
		i++;
	}
	return (1);
}