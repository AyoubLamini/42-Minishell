/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 22:04:14 by alamini           #+#    #+#             */
/*   Updated: 2024/10/13 22:20:25 by alamini          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell_exec.h"

typedef struct s_data
{
    void *data;
    struct s_data *next;
}t_data;


void dataclear(t_data **head)
{
    t_data *tmp;
    t_data *ptr;
    tmp = *head;
    while(tmp)
    {
        ptr = tmp;
        tmp = tmp->next;
        free(ptr->data);
        free(ptr);
    }
    head = NULL;
}

void *my_malloc(size_t size, int mode)
{
    static t_data *head;
    t_data *node = NULL;
    void *data = NULL;
    if(mode == 1)
    {
        data = malloc(size);
        if(!data)
            return (dataclear(&head), NULL);
        node = malloc(sizeof(t_data));
        if(!node)
            return (dataclear(&head), free(data), NULL);
        (1) && (node->data = data, node->next = head, head = node);
        return (data);
    }
    else if(!mode)
        return(dataclear(&head), NULL);
    return (write(2, "INVALID MODE\n", 13) ,NULL);
}
