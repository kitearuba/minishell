#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:30:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/21 20:30:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t g_heredoc_interrupted = 0;

static void	handle_sigint(int signum)
{
    (void)signum;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

static void	handle_sigquit(int signum)
{
    (void)signum;
    write(1, "\b\b  \b\b", 6);
}

void	setup_signal_handlers(void)
{
    signal(SIGINT, handle_sigint);
    signal(SIGQUIT, handle_sigquit);
}

void	setup_child_signals(void)
{
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}


/*
t_signal g_sig = {.pid = -1, .exit_status = 0 }; // -1 to indicate the waiting parent

// handling ctrl + c
void sig_int(int signum)
{
    (void)signum;
    if(g_sig.pid == -1)
    {
        ft_printf_fd(STDOUT_FILENO, "\n");
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
        g_sig.exit_status = 130;
    }
    else if (g_sig.pid > 0)
    {
        kill(g_sig.pid, SIGINT);
        ft_putchar_fd('\n', STDOUT_FILENO);
    }
    g_sig.sigint = 1;
}

// handling ctrl + "\"
void sig_quit(int code)
{
    char *nbr;
    (void)code;

    if (g_sig.pid == -1)
    {
        write(STDERR_FILENO, "\b\b \b\b", 6);
        g_sig.sigquit = 1;
    }
    else
        g_sig.sigquit = 1;
}

void setup_signal_handlers(void)
{
    struct sigaction sa_int;
    struct sigaction sa_quit;

    sa_int.sa_handler = sig_int;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);
    sa_quit.sa_handler = sig_quit;
    sigemptyset(&sa_quit.sa_mask);
    sa_quit.sa_flags = SA_RESTART;
    sigaction(SIGQUIT, &sa_quit, NULL);
}

void exit_on_signal(int sig)
{
    exit(128 + sig);
}

void setup_child_signals(void)
{
    signal(SIGINT, exit_on_signal);
    signal(SIGQUIT, exit_on_signal);
}

void handle_child_termination(int status)
{
    int sig;

    if (status > 255)
    {
        sig = status - 256;
        if (sig == SIGQUIT)
        {
            ft_putstr_fd("Quit (core dumped)\n", STDERR_FILENO);
        }
        else if (sig == SIGINT)
        {
            ft_putchar_fd('\n', STDERR_FILENO);
        }
        g_sig.exit_status = 128 + sig;
    }
    else
    {
        g_sig.exit_status = status;
    }
}

void reset_signal_flags(void)
{
    g_sig.sigint = 0;
    g_sig.sigquit = 0;
}
*/