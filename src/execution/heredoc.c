/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:16:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Temp switch parent signal handlers during heredoc (0=ignore, 1=restore). */
static void	hd_parent_signals(int enable)
{
	if (enable == 0)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
	}
	else
	{
		setup_signal_handlers();
	}
}

/* Read heredoc lines into wfd until limiter is met; 
 * expands $VAR when not quoted. */
static void	hd_read_loop(int wfd, const char *lim, int quoted, t_bash *bash)
{
	char	*line;
	char	*exp;

	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_strcmp(line, lim) == 0)
		{
			free(line);
			break ;
		}
		if (!quoted)
		{
			exp = hd_expand_line(line, bash);
			free(line);
			line = exp;
			if (!line)
				break ;
		}
		write(wfd, line, ft_strlen(line));
		write(wfd, "\n", 1);
		free(line);
	}
}

/* Fork the heredoc reader child and return its pid; pipefd must be open(). */
static pid_t	hd_spawn(int *pipefd, const char *lim, int quoted, t_bash *bash)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_IGN);
		close(pipefd[0]);
		hd_read_loop(pipefd[1], lim, quoted, bash);
		close(pipefd[1]);
		_exit(0);
	}
	return (pid);
}

/* Inspect child status.
 * On SIGINT: print newline, set $?=130, close rfd, return -1; else rfd. */
static int	hd_handle_status(int rfd, int status, t_bash *bash)
{
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		write(1, "\n", 1);
		close(rfd);
		bash->exit_status = 130;
		return (-1);
	}
	if (WIFSIGNALED(status))
	{
		close(rfd);
		bash->exit_status = 1;
		return (-1);
	}
	return (rfd);
}

/* -------------------------------------------------------------------------- */
/* Create a heredoc reader and return an FD connected to its output           */
/* -------------------------------------------------------------------------- */
/*
** handle_heredoc
** --------------
** Spawns a child that reads lines until 'redir->filename' delimiter.
** If delimiter was quoted, expansions are disabled. 
** On Ctrl-C, returns -1 and sets $?=130.
**
** Params:
**   redir : t_redirection* - heredoc node (uses filename + quoted)
**   bash  : t_bash*        - shell state (exit_status may be updated)
**
** Returns:
**   int                 - readable FD on success; -1 on error (SIGINT or other)
*/
int	handle_heredoc(t_redirection *redir, t_bash *bash)
{
	int		pipefd[2];
	int		status;
	pid_t	pid;

	if (!redir || !redir->filename)
		return (-1);
	if (pipe(pipefd) == -1)
		return (-1);
	hd_parent_signals(0);
	pid = hd_spawn(pipefd, redir->filename, redir->quoted, bash);
	if (pid < 0)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		hd_parent_signals(1);
		return (-1);
	}
	close(pipefd[1]);
	waitpid(pid, &status, 0);
	hd_parent_signals(1);
	return (hd_handle_status(pipefd[0], status, bash));
}
