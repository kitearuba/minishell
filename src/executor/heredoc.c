//
// Created by christian on 21/07/25.
//
#include "../../include/minishell.h"

static void	heredoc_sigint(int signum)
{
    (void)signum;
    g_heredoc_interrupted = 1;
    close(STDIN_FILENO); // Force readline to return NULL
}

static void	setup_heredoc_signals(struct sigaction *old)
{
    struct sigaction new;

    new.sa_handler = heredoc_sigint;
    sigemptyset(&new.sa_mask);
    new.sa_flags = 0;
    sigaction(SIGINT, &new, old);
}

static void	read_heredoc_input(int write_fd, const char *limiter)
{
    char	*line;

    while (1)
    {
        line = readline("> ");
        if (!line || g_heredoc_interrupted)
            break;
        if (ft_strcmp(line, limiter) == 0)
        {
            free(line);
            break;
        }
        ft_putstr_fd(line, write_fd);
        ft_putchar_fd('\n', write_fd);
        free(line);
    }
}

int	handle_heredoc(t_redirection *redir)
{
    int				pipefd[2];
    pid_t			pid;
    int				status;
    struct sigaction	old;

    if (pipe(pipefd) == -1)
        return (perror("pipe"), -1);

    pid = fork();
    if (pid == -1)
        return (perror("fork"), -1);
    if (pid == 0)
    {
        close(pipefd[0]);
        setup_heredoc_signals(&old);
        read_heredoc_input(pipefd[1], redir->filename);
        close(pipefd[1]);
        exit(g_heredoc_interrupted ? 1 : 0);
    }
    close(pipefd[1]);
    waitpid(pid, &status, 0);
    if (WIFSIGNALED(status) || WEXITSTATUS(status) != 0)
    {
        close(pipefd[0]);
        return (-1);
    }
    return (pipefd[0]);
}
