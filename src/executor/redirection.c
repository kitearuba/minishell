//
// Created by christian on 21/07/25.
//
#include "../../include/minishell.h"

#include "../../include/minishell.h"

int	apply_redirections(t_redirection *redir, t_bash *bash)
{
    (void)bash;
    int	fd;

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
        redir = redir->next;
    }
    return (0);
}
