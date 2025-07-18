#include "../../include/minishell.h"

int	ft_env(char **argv, t_bash *bash)
{
    int	i;

    (void)argv;
    i = 0;
    while (bash->env && bash->env[i])
    {
        if (ft_strchr(bash->env[i], '='))
            ft_putendl_fd(bash->env[i], STDOUT_FILENO);
        i++;
    }
    bash->exit_status = 0;
    return (0);
}
