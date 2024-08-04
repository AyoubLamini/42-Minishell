#include "minishell_exec.h"

int ex_strcmp(char *s1, char *s2)
{
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
void	ex_putstr_fd(char *s, int fd)
{
	int	i;

	if (fd < 0 || !s)
		return ;
	i = 0;
	while (s[i])
		write(fd, &s[i++], 1);
}