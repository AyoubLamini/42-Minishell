/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parssing_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/12 06:36:26 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/12 06:36:36 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"


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