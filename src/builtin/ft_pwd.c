
#include "../../include/minishell.h"

int ft_pwd(char **argv, t_bash *bash)
{
    char    *cwd;

    (void)argv;
    cwd = getcwd(NULL, 0);
    if (!cwd)
    {
        perror("pwd");
        bash->exit_status = 1;
        return (1);
    }
    ft_putendl_fd(cwd, STDOUT_FILENO);
    free(cwd);
    bash->exit_status = 0;
    return (0);
}