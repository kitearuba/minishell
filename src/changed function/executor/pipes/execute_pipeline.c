/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 16:37:45 by bsamy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	child_process(
	t_command *cmd, int input_fd, int output_fd, t_bash *bash)
{
	int	st;

	if (dup2(input_fd, STDIN_FILENO) == -1
		|| dup2(output_fd, STDOUT_FILENO) == -1)
		free_all_and_exit(bash, 1);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
	setup_child_signals();
	st = 0;
	if (cmd->redirection)
	{
		st = apply_redirections(cmd->redirection, bash);
		if (st != 0)
			free_all_and_exit(bash, st);
	}
	execve_cmd(cmd->argv, bash->env, bash);
}

static void	pipe_child(
	t_command *cmd, int *input_fd, int pipefd[2], t_bash *bash)
{
	pid_t	pid;

	if (pipe(pipefd) == -1)
		free_all_and_exit(bash, 1);
	pid = fork();
	if (pid < 0)
		free_all_and_exit(bash, 1);
	else if (pid == 0)
		child_process(cmd, *input_fd, pipefd[1], bash);
	close(pipefd[1]);
	if (*input_fd != STDIN_FILENO)
		close(*input_fd);
	*input_fd = pipefd[0];
}

static int	wait_for_pipeline(pid_t last_pid, t_bash *bash)
{
	int	status;

	waitpid(last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFSIGNALED(status))
		bash->exit_status = 128 + WTERMSIG(status);
	else if (WIFEXITED(status))
		bash->exit_status = WEXITSTATUS(status);
	else
		bash->exit_status = 1;
	return (bash->exit_status);
}

int	execute_pipeline(t_command *cmd, t_bash *bash)
{
	int		pipefd[2];
	int		input_fd;
	pid_t	last_pid;

	input_fd = STDIN_FILENO;
	last_pid = -1;
	while (cmd->next)
	{
		pipe_child(cmd, &input_fd, pipefd, bash);
		cmd = cmd->next;
	}
	last_pid = fork();
	if (last_pid < 0)
		free_all_and_exit(bash, 1);
	else if (last_pid == 0)
		child_process(cmd, input_fd, STDOUT_FILENO, bash);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	return (wait_for_pipeline(last_pid, bash));
}
