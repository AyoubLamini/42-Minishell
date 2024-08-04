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


// comannds 
int execute_command(t_command *command, char *path);

// builtins
int     cd(t_command *command);
int     echo(t_command *command);
int     env(t_command *command);
// redirections 
int handle_redirection(t_command *command, t_exec *file_d);


#endif