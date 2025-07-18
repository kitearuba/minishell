
#include "../../include/minishell.h"

static int    is_n_flag(char *arg)
{
    int    i;

    if (!arg || arg[0] != '-' || arg[1] != 'n')
        return (0);
    i = 2;
    while (arg[i] == 'n')
        i++;
    return (arg[i] == '\0');
}

int ft_echo(char **argv, t_bash *bash)
{
    int    i;
    int    newline;

    i = 1;
    newline = 1;
    while (argv[i] && is_n_flag(argv[i]))
    {
        newline = 0;
        i++;
    }
    while (argv[i])
    {
        ft_putstr_fd(argv[i], STDOUT_FILENO);
        if (argv[i+ 1])
            ft_putchar_fd(' ', STDOUT_FILENO);
        i++;
    }
    if (newline)
        ft_putchar_fd('\n', STDOUT_FILENO);
    bash->exit_status = 0;
    return (0);
}