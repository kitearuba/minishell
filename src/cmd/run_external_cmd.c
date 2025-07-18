#include "../../include/minishell.h"

/* ************************************************************************** */
/*                            print_cmd_not_found                             */
/* ************************************************************************** */
/*  Description:                                                              */
/*  - Prints an error message to stderr indicating the command was not found.*/
/* ************************************************************************** */
static void	print_cmd_not_found(char *cmd)
{
	ft_printf_fd(2, "minishell: command not found: %s\n", cmd);
}

/* ************************************************************************** */
/*                             exec_external                                  */
/* ************************************************************************** */
/*  Description:                                                              */
/*  - Executes external commands using execve in a child process.            */
/* ************************************************************************** */
int	exec_external(char **args, t_bash *bash)
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

/* ************************************************************************** */
/*                            run_external_cmd                                */
/* ************************************************************************** */
/*  Description:                                                              */
/*  - Main dispatcher: decides if a command is a built-in or external.        */
/* ************************************************************************** */
int	run_external_cmd(t_command *cmd, t_bash *bash)
{
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);

	if (is_builtin(cmd->argv[0]))
		return (run_builtin(cmd->argv, bash));
	else
		return (exec_external(cmd->argv, bash));
}
