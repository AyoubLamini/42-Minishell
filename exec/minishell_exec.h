#ifndef MINISHELL_EXEC_H
#define MINISHELL_EXEC_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../includes/minishell.h"
#include <errno.h>
#include <string.h>

typedef struct s_exec
{
    char *path;
    int in;
    int out;
} t_exec;


// utils
char	**ex_split(char const *s, char c);
int     ex_strcmp(char *s1, char *s2);
char	*ex_strjoin(char *s1, char *s2);
size_t	ex_strlen(const char *s);
char	*ex_strdup(const char *s1);
void    print_double_pointer(char **double_ptr);
void	ex_putstr_fd(char *s, int fd);
void    print_error(char *cmd, char *path, char *error);
int	    ft_strstr(char *haystack, char *needle);
// list utils
t_env   *env_vars_copy(t_env **env_vars);
int	    list_size(t_env *lst);
void    sort_vars(t_env **env_vars);
void	update_var(t_env *env, char *env_key, char *new_value);
char	*get_env(t_env *env, char *env_key);
// comannds 
void check_command(t_command *command, t_env **env_vars);


// builtins
int     cd(t_command *command, t_env *env_vars);
int     echo(t_command *command);
void    env(t_env *env_vars);
void    export(t_command *cmds, t_env **env_vars);
void    unset(t_command *cmds, t_env **env_vars);
// redirections 
int handle_redirection(t_command *command, t_exec *file_d);
// piping 
void piping(t_command *command, t_env **env_vars, int *id);


#endif