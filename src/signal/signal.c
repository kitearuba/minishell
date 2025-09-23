/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:30:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/23 22:41:04 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

volatile sig_atomic_t	g_sig = 0;

volatile sig_atomic_t	*get_sigint_flag(void)
{
	return (&g_sig);
}

/* Prompt: redraw line (bash-like), SA_RESTART on */
static void	prompt_sigint(int signum)
{
	(void)signum;
	g_sig = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = prompt_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa, 0);
	signal(SIGQUIT, SIG_IGN);
}

/* Heredoc: stop readline immediately, no redisplay, no SA_RESTART */
static void	heredoc_sigint(int signum)
{
	extern int	rl_done;

	(void)signum;
	g_sig = SIGINT;
	write(1, "\n", 1);
	rl_replace_line("", 0);
	rl_done = 1;
}

void	setup_heredoc_signals(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = heredoc_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, 0);
	signal(SIGQUIT, SIG_IGN);
}

void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
