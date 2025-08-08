/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external_cmd.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/05/20 16:45:41 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_cmd_not_found(char *cmd)
{
	ft_printf_fd(2, "minishell: command not found: %s\n", cmd);
}

static int	spawn_child(char *cmd_path, char **args, t_bash *bash)
{
	pid_t	pid;
	int		status;

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

int	exec_external(char **args, t_bash *bash)
{
	char	*cmd_path;

	if (!args || !args[0])
		return (127);
	cmd_path = get_cmd_path(args[0], bash->env);
	if (!cmd_path)
	{
		print_cmd_not_found(args[0]);
		bash->exit_status = 127;
		return (127);
	}
	return (spawn_child(cmd_path, args, bash));
}
