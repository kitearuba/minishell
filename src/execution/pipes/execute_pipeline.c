/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipeline.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:12:27 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Execute one stage of a pipeline in a child                                 */
/* -------------------------------------------------------------------------- */
/*
** child_process
** -------------
** Wire input_fd -> STDIN and output_fd -> STDOUT, apply redirections for the
** given command, set child signal handlers, and exec the program. On any
** failure before exec, exit(1); if redirections fail, exit(st) where st is the
** redirection error code (e.g., 130 for heredoc SIGINT).
**
** Params:
**   cmd       : t_command* - pipeline stage (argv + redirections)
**   input_fd  : int        - fd to dup to STDIN
**   output_fd : int        - fd to dup to STDOUT
**   bash      : t_bash*    - shell state for env and cleanup on fatal errors
*/
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

/* -------------------------------------------------------------------------- */
/* Spawn a pipeline stage (non-final) and pass its read end downstream        */
/* -------------------------------------------------------------------------- */
/*
** pipe_child
** ----------
** Create a pipe, fork a child for 'cmd' writing to pipefd[1], and set *input_fd
** to the read end (pipefd[0]) for the next stage. Parent closes write end and
** previous *input_fd (if not STDIN).
**
** Params:
**   cmd      : t_command* - current stage
**   input_fd : int*       - in/out; becomes next stage's input fd
**   pipefd   : int[2]     - temp pipe buffer (created here)
**   bash     : t_bash*    - shell state for fatal cleanup
*/
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

/* -------------------------------------------------------------------------- */
/* Reap the pipeline and compute the final exit status                        */
/* -------------------------------------------------------------------------- */
/*
** wait_for_pipeline
** -----------------
** Waits for 'last_pid' (the rightmost stage). Then reaps remaining children.
** Sets bash->exit_status to:
**   - 128+signal if the last stage was terminated by a signal
**   - WEXITSTATUS otherwise
**   - 1 on unexpected wait states
**
** Returns the same status stored in bash->exit_status.
*/
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

/* -------------------------------------------------------------------------- */
/* Orchestrate an entire pipeline                                             */
/* -------------------------------------------------------------------------- */
/*
** execute_pipeline
** ----------------
** For each stage except the last, fork a child that writes into a new pipe and
** update the read end for the next stage. For the final stage, fork a child
** connected to STDOUT. Then wait for the rightmost child and reap others.
**
** Params:
**   cmd  : t_command* - head of the pipeline linked list
**   bash : t_bash*    - shell state (env + exit_status updated here)
**
** Returns:
**   int               - final pipeline exit status (stored in bash->exit_status)
**
** Notes:
** - Child signals are set via setup_child_signals(); parent uses default/prompt
**   handlers configured elsewhere.
** - Redirections are applied inside children so each stage has its own view.
*/
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
