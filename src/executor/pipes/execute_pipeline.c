/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/20 14:45:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

static void	child_process(t_command *cmd, int input_fd, int output_fd, t_bash *bash)
{
	if (dup2(input_fd, STDIN_FILENO) == -1 || dup2(output_fd, STDOUT_FILENO) == -1)
		free_all_and_exit(bash, 1);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	if (output_fd != STDOUT_FILENO)
		close(output_fd);
    setup_child_signals();
    if (apply_redirections(cmd->redirection, bash))
		free_all_and_exit(bash, 1);
	execve_cmd(cmd->argv, bash->env, bash);
}

int	execute_pipeline(t_command *cmd, t_bash *bash)
{
	int		pipefd[2];
	int		input_fd = STDIN_FILENO;
	pid_t	last_pid = -1;
	pid_t	pid;
    int status;

	while (cmd->next)
	{
		if (pipe(pipefd) == -1)
			free_all_and_exit(bash, 1);
		pid = fork();
		if (pid < 0)
			free_all_and_exit(bash, 1);
		else if (pid == 0)
			child_process(cmd, input_fd, pipefd[1], bash);
		close(pipefd[1]);
		if (input_fd != STDIN_FILENO)
			close(input_fd);
		input_fd = pipefd[0];
		cmd = cmd->next;
	}
	last_pid = fork();
	if (last_pid < 0)
		free_all_and_exit(bash, 1);
	else if (last_pid == 0)
		child_process(cmd, input_fd, STDOUT_FILENO, bash);
	if (input_fd != STDIN_FILENO)
		close(input_fd);
	waitpid(last_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	bash->exit_status = WEXITSTATUS(status);
	return (bash->exit_status);
}
