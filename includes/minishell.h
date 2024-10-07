/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/13 22:27:24 by ybouyzem          #+#    #+#             */
/*   Updated: 2024/10/07 06:31:50 by ybouyzem         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef MINISHEL_H
# define MINISHEL_H

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
#include "../libft/libft.h"
#include <signal.h>



#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BOLD_GREEN "\x1b[1;32m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

#define AMBIGOUS_INDEX -1

typedef struct s_env {
	char	*key;
	char	*value;
	struct s_env *next;
} t_env;

typedef struct s_command {
	char    **cmd;
	char    **redirection;
	char 	*last_file;
	struct s_command *next;
} t_command;

typedef struct s_heredoc {
    char *delimiter;
    char *file;
    struct s_heredoc *next;
} t_heredoc;

typedef struct s_path
{
    int exit_status;
    int is_forked;
    int fd_in;
    int fd_out;
    struct s_heredoc *heredoc;  
} t_path;

enum //enumeration type
{
	ATTR_SET,  
	ATTR_GET,  
	ATTR_CHG 
};

enum
{
	SET_SIG,
	UNSET_SIG
};

void	syntax_error_messages(int code);
int		is_redirection(char *str, int index);
int		check_syntax(char *input);
int		check_quotes(char	*input);
int		ft_isspace(char c);
void	remove_spaces(char **input);
char	*add_spaces(char *input);
char	**split_args(char *s, char c);
void	ft_check_quotes(int *single_quote, int *double_quote, char c);
t_command	*split_cmds(char **args, t_env *env_vars, t_path *path);
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
char	**expanding_cmd(t_env *envs, char *old_cmd, t_path *path);
char	**expanding_red(t_env *envs, char *old_cmd, t_path *path);
t_env   *full_envs(char **env);
t_env	*new_variable(char *env_key, char *env_value);
char	*get_env_variable(t_env *env, char *env_key);
void	add_env_back(t_env **envs, t_env *new);
void 	delete_env(t_env **env, char *env_key);
char 	*get_str(char *var, char *type);
void	printstrs(char **map);
void	print_envs(t_env *envs);
char    **expanding_split(char  *old_cmd);
int		check_will_splited(t_env *envs, char *str, t_path *path);
char	**join_double_strs_with_str(char **s1, char *s2);
char	**join_two_double_strs(char **s1, char **s2);
char	*normal_process(t_env *envs, char *str);
int	check_is_joinable(char **cmd, int index);
int	ft_check_space_in_cmd(char *str);
char	*double_quotes_process(t_env *envs, char *str, t_path *path);
char	*single_quotes_process(char *str);
int	is_only_spaces(char *str);


// execution
void execute(t_command *command, t_env **env_vars, t_path *path);
void setup_signals(t_path *path, int action);


// heredoc
char	*expanding_herdoc(t_env *envs, char *str, t_path path);
char    *expanding_cmd_herdoc(t_env *envs, char *old_cmd, t_path path);
void 	clear_herdocs(t_path *path);
int 	handle_herdoc(t_command *command, t_path *path, t_env **envs);
t_heredoc *lst_heredoc_new(char *delimiter, char *file);
void    lst_heredoc_add_back(t_heredoc **lst, t_heredoc *new);
char	*get_right_delimeter(char *s);
int	check_will_expanded(char *delimter);
#endif