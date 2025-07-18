
#include "../../include/minishell.h"

static int	is_numeric(const char *str)
{
    int	i;

    if (!str || !*str)
        return (0);
    if (str[0] == '+' || str[0] == '-')
        str++;
    i = 0;
    while (str[i])
    {
        if (!ft_isdigit(str[i]))
            return (0);
        i++;
    }
    return (1);
}

int	ft_exit(char **argv, t_bash *bash)
{
    long long	code;
    int			arg_count;

    arg_count = 0;
    while (argv && argv[arg_count])
        arg_count++;
    ft_putendl_fd("exit", STDERR_FILENO);
    if (arg_count == 1)
        exit(bash->exit_status);
    if (!is_numeric(argv[1]))
    {
        ft_printf_fd(STDERR_FILENO, "minishell: exit: %s: numeric argument required\n", argv[1]);
        exit(255);
    }
    if (arg_count > 2)
    {
        ft_putendl_fd("minishell: exit: too many arguments", STDERR_FILENO);
        return (1);
    }
    code = ft_atoll(argv[1]);
    exit((unsigned char)code);
}
