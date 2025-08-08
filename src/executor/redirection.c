/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 14:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/08 03:15:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	apply_fd(t_redirection *redir, int fd)
{
	if (fd < 0)
		return (perror(redir->filename), 1);
	if (redir->type == REDIRECT_IN || redir->type == HEREDOC)
	{
		if (dup2(fd, STDIN_FILENO) < 0)
			return (perror("dup2"), 1);
	}
	else if (redir->type == REDIRECT_OUT || redir->type == REDIRECT_APPEND)
	{
		if (dup2(fd, STDOUT_FILENO) < 0)
			return (perror("dup2"), 1);
	}
	close(fd);
	return (0);
}

int	apply_redirections(t_redirection *redir, t_bash *bash)
{
	int	fd;

	(void)bash;
	while (redir)
	{
		if (redir->type == REDIRECT_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == REDIRECT_OUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIRECT_APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == HEREDOC)
			fd = handle_heredoc(redir);
		if (apply_fd(redir, fd))
			return (1);
		redir = redir->next;
	}
	return (0);
}
