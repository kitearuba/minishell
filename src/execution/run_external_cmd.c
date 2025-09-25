/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 16:08:20 by bsamy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_cmd_not_found(const char *cmd, t_bash *bash)
{
	char	*path_val;

	path_val = ft_getenv(bash->env, "PATH");
	if (!path_val || !*path_val)
		ft_printf_fd(STDERR_FILENO,
			"minishell: %s: No such file or directory\n", cmd);
	else
		ft_printf_fd(STDERR_FILENO,
			"minishell: %s: command not found\n", cmd);
}

static int	wait_and_set_status(pid_t pid, t_bash *bash)
{
	int	status;

	if (waitpid(pid, &status, 0) < 0)
	{
		perror("minishell: waitpid");
		bash->exit_status = 1;
		return (1);
	}
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		bash->exit_status = 128 + WTERMSIG(status);
	}
	else if (WIFEXITED(status))
		bash->exit_status = WEXITSTATUS(status);
	else
		bash->exit_status = 1;
	return (bash->exit_status);
}

static int	spawn_child(const char *cmd_path, char **args, t_bash *bash)
{
	pid_t	pid;

	parent_ignore_signals();
	pid = fork();
	if (pid == 0)
	{
		setup_child_signals();
		execve((char *)cmd_path, args, bash->env);
		perror("minishell: execve");
		_exit(126);
	}
	else if (pid < 0)
	{
		perror("minishell: fork");
		parent_restore_prompt_signals();
		return (1);
	}
	wait_and_set_status(pid, bash);
	parent_restore_prompt_signals();
	return (bash->exit_status);
}

static int	run_path_cmd(char *path, char **args, t_bash *bash)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		ft_printf_fd(2, "minishell: %s: No such file or directory\n", path);
		bash->exit_status = 127;
		return (127);
	}
	if (S_ISDIR(st.st_mode))
	{
		ft_printf_fd(2, "minishell: %s: Is a directory\n", path);
		bash->exit_status = 126;
		return (126);
	}
	if (access(path, X_OK) == -1)
	{
		ft_printf_fd(2, "minishell: %s: Permission denied\n", path);
		bash->exit_status = 126;
		return (126);
	}
	return (spawn_child(path, args, bash));
}

int	exec_external(char **args, t_bash *bash)
{
	char	*cmd_path;

	if (!args || !args[0])
		return (127);
	if (ft_strchr(args[0], '/'))
		return (run_path_cmd(args[0], args, bash));
	cmd_path = get_cmd_path(args[0], bash->env);
	if (!cmd_path)
	{
		print_cmd_not_found(args[0], bash);
		bash->exit_status = 127;
		return (127);
	}
	bash->exit_status = spawn_child(cmd_path, args, bash);
	free(cmd_path);
	return (bash->exit_status);
}
