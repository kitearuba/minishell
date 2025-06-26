#include "../../include/minishell.h"

static void	print_cmd_not_found(char *cmd)
{
    ft_putstr_fd("minishell: command not found: ", 2);
    ft_putendl_fd(cmd, 2);
}

int exec_external(char **args, t_bash *bash)
{
    char	*cmd_path;
    pid_t	pid;
    int		status;

    if (!args || !args[0])
        return (127);

    cmd_path = get_cmd_path(args[0], bash->env);
    if (!cmd_path)
    {
        print_cmd_not_found(args[0]);
        bash->exit_status = 127;
        return (127);
    }

    pid = fork();
    if (pid == 0)
    {
        execve(cmd_path, args, bash->env);
        perror("minishell: execve");
        exit(1);
    }
    else if (pid < 0)
    {
        perror("minishell: fork");
        free(cmd_path);
        return (1);
    }

    waitpid(pid, &status, 0);
    bash->exit_status = WEXITSTATUS(status);
    free(cmd_path);
    return (bash->exit_status);
}