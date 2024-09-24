#ifndef MINISHELL_EXEC_H
#define MINISHELL_EXEC_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include "../includes/minishell.h"
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <sys/stat.h>

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
int     ft_is_numeric(char *s);
int     occur_alpha(char *str, int c);
// list utils
t_env   *env_vars_copy(t_env **env_vars);
int	    list_size(t_env *lst);
void    sort_vars(t_env **env_vars);
void	update_var(t_env *env, char *env_key, char *new_value);
char	*get_env_value(t_env *env, char *env_key);
char	*get_env_key(t_env *env, char *env_key);
char    **envp_array(t_env *vars);

// other utils
void exit_status(int status, t_path *path);

// comannds 
void check_command(t_command *command, t_env **env_vars, t_path *path);


// builtins
int     cd(t_command *command, t_env *env_vars);
int     echo(t_command *command);
int     env(t_env *env_vars);
int     pwd();
int     export(t_command *cmds, t_env **env_vars);
int     unset(t_command *cmds, t_env **env_vars);
void    exit_shell(t_command *command, t_path *path);
// redirections 
int handle_redirection(t_command *command, t_exec *file_d, t_path *path);
void    ft_heredoc(t_command *command, t_path *path, char *delimiter);
// piping 
void piping(t_command *command, t_env **env_vars, int *input_fd, t_exec *file_d, t_path *path);


#endif