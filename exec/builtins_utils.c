#include "../includes/minishell_exec.h"

t_env *env_vars_copy(t_env **env_vars)
{
    t_env *vars_copy;
    t_env *tmp;

    tmp = *env_vars;
    vars_copy = NULL;
    while (tmp)
    {
        add_env_back(&vars_copy, new_variable(ft_strdup(tmp->key), ft_strdup(tmp->value)));
        tmp = tmp->next;
    }
    return (vars_copy);
}

void sort_vars(t_env **env_vars)
{
    int i;
    t_env *current;
    char  *key_tmp;
    char *value_tmp;

    i = 0;
    current = *env_vars;
    while (i++ < list_size(*env_vars))
    {
        current = *env_vars;
        while (current->next)
        {
            if (ex_strcmp(current->key, current->next->key) > 0)
            {
                key_tmp = current->key;
                value_tmp = current->value;
                current->key = current->next->key;
                current->value = current->next->value;
                current->next->key = key_tmp;
                current->next->value = value_tmp; 
            }
            current = current->next;
        }
    }
    return ;
}

int	list_size(t_env *lst)
{
	int		i;
	t_env	*p;

	p = lst;
	i = 0;
	while (p != NULL)
	{
		i++;
		p = p->next;
	}
	return (i);
}
void	update_var(t_env *env, char *env_key, char *new_value)
{
	t_env	*tmp;
	
	tmp = env;
	while (tmp)
	{
		if (ex_strcmp(tmp->key, env_key) == 0)
        {
            if (tmp->value)
                free(tmp->value);
            tmp->value = new_value;
            return ;
        }
		tmp = tmp->next;
	}
	return ;
}
char	*get_env_value(t_env *env, char *env_key) // added this function
{
	t_env	*tmp;
	
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, env_key) == 0)
			return (tmp->value);
		tmp = tmp->next;
	}
	return (NULL);
}
char	*get_env_key(t_env *env, char *env_key) // added this function
{
	t_env	*tmp;
	
	tmp = env;
	while (tmp)
	{
		if (ft_strcmp(tmp->key, env_key) == 0)
			return (tmp->key);
		tmp = tmp->next;
	}
	return (NULL);
}
char **envp_array(t_env *vars)
{
    t_env *tmp;
    char **envp;
    int i;
    char *str;

    i = 0;
    tmp = vars;
    envp = (char **)malloc(sizeof(char *) * (list_size(tmp) + 1));
    while (tmp)
    {
        str = ex_strjoin(ft_strdup("="), tmp->value);
        envp[i] = ex_strjoin(ft_strdup(tmp->key), str);
        i++;
        free(str);
        tmp = tmp->next;
    }
    envp[i] = NULL;
    return (envp);
}