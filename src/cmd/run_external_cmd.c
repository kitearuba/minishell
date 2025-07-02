#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                        print_cmd_not_found                                */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Prints an error message to stderr indicating that the specified         */
/*    command was not found, using ft_printf_fd().                            */
/*                                                                            */
/*  Parameters:                                                               */
/*  - cmd: the command that was not found.                                    */
/*                                                                            */
/*  Return:                                                                   */
/*  - None.                                                                   */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Outputs to fd 2 (stderr) for proper error reporting.                    */
/*                                                                            */
/* ************************************************************************** */
static void	print_cmd_not_found(char *cmd)
{
    ft_printf_fd(2, "minishell: command not found: %s\n", cmd);
}


/* ************************************************************************** */
/*                                                                            */
/*                            exec_external                                  */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Executes an external command by searching for its executable path,      */
/*    forking a child process, and running execve with the command arguments. */
/*                                                                            */
/*  Parameters:                                                               */
/*  - args: NULL-terminated array of command arguments.                       */
/*  - bash: pointer to the bash struct containing environment and status.     */
/*                                                                            */
/*  Return:                                                                   */
/*  - Exit status of the executed command.                                    */
/*  - Returns 127 if the command is not found, or 1 on fork/exec error.       */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Prints "command not found" errors with print_cmd_not_found() using      */
/*    ft_printf_fd() to stderr.                                               */
/*  - Uses perror() for fork or execve errors to display system messages.     */
/*  - Updates bash->exit_status with the command’s exit code.                 */
/*                                                                            */
/* ************************************************************************** */
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