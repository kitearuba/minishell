//
// Created by christian on 21/07/25.
//
#include "../../include/minishell.h"

int	handle_heredoc(t_redirection *redir)
{
    char	*line;
    int		pipefd[2];

    if (pipe(pipefd) == -1)
        return (perror("pipe"), -1);

    while (1)
    {
        line = readline("> ");
        if (!line)
            break;
        if (ft_strcmp(line, redir->filename) == 0)
        {
            free(line);
            break;
        }
        ft_putstr_fd(line, pipefd[1]);
        ft_putchar_fd('\n', pipefd[1]);
        free(line);
    }
    close(pipefd[1]);
    return (pipefd[0]);
}
