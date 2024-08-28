/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 22:27:24 by ybouyzem          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/27 12:27:26 by alamini          ###   ########.fr       */
=======
/*   Updated: 2024/08/28 03:26:07 by ybouyzem         ###   ########.fr       */
>>>>>>> 6220da4e5d5fbccd3f5c197c3c7a16e5d6680b9c
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHEL_H
# define MINISHEL_H

#include <stdio.h>
#include <unistd.h>

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../libft/libft.h"


#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BOLD_GREEN "\x1b[1;32m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define PID getpid()

typedef struct s_env {
	char	*key;
	char	*value;
	struct s_env *next;
} t_env;

typedef struct s_command {
	char    **cmd;
	char    **redirection;
	struct s_command *next;
} t_command;

typedef struct s_path
{
	int exit_status;
	char *path;
}	t_path;

int		is_redirection(char *str, int index);
int		check_syntax(char *input);
int		check_quotes(char	*input);
int		ft_isspace(char c);
void	remove_spaces(char **input);
char	*add_spaces(char *input);
char	**split_args(char *s, char c);
void	ft_check_quotes(int *single_quote, int *double_quote, char c);
t_command	*split_cmds(char **args, t_env *env_vars, int pid);
void *ft_myfree(char **result, int index);
int	ft_strslen(char **map);
int	ft_strcmp(char *s1, char *s2);
char	*concat_strs(char* s1, char* s2);

// list
void	lstadd_back(t_command **lst, t_command *new);
int		lstsize(t_command *lst);
t_command	*lstlast(t_command *lst);
t_command	*lstnew(char **cmd, char **red);
void	print_list(t_command *lst);


//free memory
void	free_cmds(t_command *cmds);
void	free_strs(char **strs);
void	free_envs(t_env *envs);

//expanding
char	*expanding(t_env *envs, char *old_cmd, int pid);
t_env   *full_envs(char **env);
t_env	*new_variable(char *env_key, char *env_value);
char	*get_env_variable(t_env *env, char *env_key);
void	add_env_back(t_env **envs, t_env *new);
void 	delete_env(t_env **env, char *env_key);
char 	*get_str(char *var, char *type);
void	printstrs(char **map);
void	print_envs(t_env *envs);
<<<<<<< HEAD
char    **expanding_split(char *old_cmd, int pid);
=======
char    **expanding_split(char  *old_cmd);

>>>>>>> 6220da4e5d5fbccd3f5c197c3c7a16e5d6680b9c

// execution
void execute(t_command *command, t_env **env_vars);
#endif