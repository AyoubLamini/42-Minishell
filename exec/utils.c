#include "minishell_exec.h"

int ex_strcmp(char *s1, char *s2)
{
    if (!s1 || !s2)
        return (-1);
    int i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return (s1[i] - s2[i]);

}
void print_double_pointer(char **double_ptr)
{
    int i = 0;
    int j = 0;
    while (double_ptr[i])
    {
        j = 0;
        while (double_ptr[i][j])
        {
            printf("%c", double_ptr[i][j]);
            j++;
        }
        i++;
    }
}
void print_error(char *cmd, char *path, char *error)
{
    ft_putstr_fd("minishell: ", 2);
    if (cmd)
        ft_putstr_fd(cmd, 2);
    ft_putstr_fd(": ", 2);
    if (path)
    {
    ft_putstr_fd(path, 2);
    ft_putstr_fd(" ", 2);
    }
    if (error)
        ft_putstr_fd(error, 2);
    ft_putstr_fd("\n", 2);
}
void	ex_putstr_fd(char *s, int fd)
{
	int	i;

	if (fd < 0 || !s)
		return ;
	i = 0;
	while (s[i])
		write(fd, &s[i++], 1);
}