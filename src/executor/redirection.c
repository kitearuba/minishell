/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 16:06:17 by bsamy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	apply_fd(t_redirection *r, int fd)
{
	if (fd < 0)
		return (perror(r->filename), 1);
	if (r->type == REDIRECT_IN || r->type == HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
			return (perror("dup2"), 1);
	}
	else if (r->type == REDIRECT_OUT || r->type == REDIRECT_APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
			return (perror("dup2"), 1);
	}
	close(fd);
	return (0);
}

static int	open_fd_for_redir(t_redirection *r)
{
	if (r->type == REDIRECT_IN)
		return (open(r->filename, O_RDONLY));
	if (r->type == REDIRECT_OUT)
		return (open(r->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	return (open(r->filename, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

static int	heredoc_to_stdin(t_redirection *r, t_bash *bash)
{
	int	fd;

	fd = handle_heredoc(r, bash);
	if (fd < 0)
	{
		if (bash->exit_status == 130)
			return (130);
		if (bash->exit_status == 0)
			bash->exit_status = 1;
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) < 0)
	{
		close(fd);
		perror("dup2");
		if (bash->exit_status == 0)
			bash->exit_status = 1;
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_redirection *r, t_bash *bash)
{
	int	fd;
	int	st;

	while (r)
	{
		if (r->type == HEREDOC)
		{
			st = heredoc_to_stdin(r, bash);
			if (st != 0)
				return (st);
			r = r->next;
			continue ;
		}
		fd = open_fd_for_redir(r);
		if (apply_fd(r, fd))
		{
			bash->exit_status = 1;
			return (1);
		}
		r = r->next;
	}
	return (0);
}
