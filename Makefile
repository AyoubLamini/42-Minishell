# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ybouyzem <ybouyzem@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/13 22:26:51 by ybouyzem          #+#    #+#              #
#    Updated: 2024/08/11 14:57:41 by ybouyzem         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# FILEUTILS = lib/utils_file.c  lib/function_utils.c lib/split_utils.c  lib/protaction.c \
# 			lib/clean_utils.c 

# FILEPARCIN = parcin/parcin_file.c 

LIB_FILES	:= libft/ft_isalpha.c libft/ft_isdigit.c libft/ft_isalnum.c libft/ft_isascii.c libft/ft_isprint.c libft/ft_strlen.c libft/ft_memset.c \
	libft/ft_bzero.c libft/ft_memcpy.c libft/ft_memmove.c libft/ft_strlcpy.c libft/ft_strlcat.c libft/ft_toupper.c libft/ft_tolower.c libft/ft_strchr.c \
	libft/ft_strrchr.c libft/ft_strncmp.c libft/ft_memchr.c libft/ft_memcmp.c libft/ft_strnstr.c libft/ft_atoi.c libft/ft_calloc.c libft/ft_strdup.c \
	libft/ft_substr.c libft/ft_strjoin.c libft/ft_strtrim.c libft/ft_split.c libft/ft_itoa.c libft/ft_strmapi.c libft/ft_striteri.c libft/ft_putchar_fd.c \
	libft/ft_putstr_fd.c libft/ft_putendl_fd.c libft/ft_putnbr_fd.c libft/ft_lstnew_bonus.c libft/ft_lstadd_front_bonus.c libft/ft_lstsize_bonus.c libft/ft_lstlast_bonus.c libft/ft_lstadd_back_bonus.c libft/ft_lstdelone_bonus.c \
	libft/ft_lstclear_bonus.c libft/ft_lstiter_bonus.c libft/ft_lstmap_bonus.c




FILE = src/main.c src/check_syntax.c src/parssing_utils.c src/split_args.c src/split_cmds.c src/list_utils.c src/expanding.c src/expanding_utils.c\
	exec/commands.c exec/builtins.c exec/builtins_utils.c exec/ft_split.c exec/ft_strjoin.c exec/redrection.c exec/utils.c exec/main_exec.c\
	src/free_memory.c $(LIB_FILES)
	

CFLAGS = -I/usr/local/opt/readline/include

LDFLAGS = -L/usr/local/opt/readline/lib -lreadline

FLAG = cc

# GFLAG = $(FLAG) -Wall -Wextra -Werror $(CFLAGS)
GFLAG = $(FLAG) -Wall -Wextra -Werror $(CFLAGS) 

OBJECTFILES = $(FILE:.c=.o)

PROGRAM = minishell

%.o: %.c includes/minishell.h exec/minishell_exec.h
	$(GFLAG) -o $@ -c $<

$(PROGRAM): $(OBJECTFILES)
	$(FLAG) $(LDFLAGS) $(OBJECTFILES) -o $(PROGRAM)


all: $(PROGRAM)

clean:
	rm -f $(OBJECTFILES)

fclean: clean
	rm -f $(PROGRAM)

re: fclean all

.PHONY: all clean fclean re