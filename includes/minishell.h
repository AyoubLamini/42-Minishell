/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alamini <alamini@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 22:27:24 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/08/09 14:52:50 by alamini          ###   ########.fr       */
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

int		is_redirection(char *str, int index);
int		check_syntax(char *input);
int		check_quotes(char	*input);
int		ft_isspace(char c);
void	remove_spaces(char **input);
char	*add_spaces(char *input);
char	**split_args(char *s, char c);
void	ft_check_quotes(int *single_quote, int *double_quote, char c);
t_command	*split_cmds(char **args);
void *ft_myfree(char **result, int index);
int	ft_strslen(char **map);

// list
void	lstadd_back(t_command **lst, t_command *new);
int		lstsize(t_command *lst);
t_command	*lstlast(t_command *lst);
t_command	*lstnew(char **cmd, char **red);
void	print_list(t_command *lst);


//free memory
void	free_cmds(t_command **cmds);
void	free_strs(char **strs);

//expanding
t_env   *expanding(char **env);
t_env	*new_variable(char *env_key, char *env_value);
char	*get_env_variable(t_env *env, char *env_key);
void	add_env_back(t_env **envs, t_env *new);
void 	delete_env(t_env **env, char *env_key);
char *get_str(char *var, char *type);
void	printstrs(char **map);

// execution 
void execute(t_command *command, t_env **env_vars);
#endif