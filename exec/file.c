#include "../includes/minishell.h"
#include "../includes/minishell_exec.h"

int main()
{

    char *ptr;
    char *line;
    line = NULL;
    ptr = NULL;
    while (1)
    {
        line = readline("minishell $> ");
        ptr = my_malloc(20, 1);
        printf("Hello\n");
        my_malloc(0, 0);
    }
}