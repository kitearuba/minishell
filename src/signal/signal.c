/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 20:30:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:24:42 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t	g_sig = 0;

/* -------------------------------------------------------------------------- */
/* Global SIGINT flag accessor (set by prompt handler)                        */
/* -------------------------------------------------------------------------- */
/*
** get_sigint_flag
** ---------------
** Returns the address of the global sigint flag. The main loop checks and
** clears this to translate Ctrl-C at the prompt into $? = 130 behavior.
**
** Returns:
**   volatile sig_atomic_t*  - pointer to global flag
*/
volatile sig_atomic_t	*get_sigint_flag(void)
{
	return (&g_sig);
}

/* -------------------------------------------------------------------------- */
/* Prompt SIGINT handler (Ctrl-C) — bash-like behavior                        */
/* -------------------------------------------------------------------------- */
/*
** prompt_sigint (file-local)
** --------------------------
** Async handler for SIGINT while waiting at the prompt:
**   - Set global flag (read by main loop)
**   - Print a newline
**   - Clear current input line and redisplay prompt (readline helpers)
**
** Notes:
** - Uses only async-signal-safe calls plus readline's helpers (see review note).
*/
static void	prompt_sigint(int signum)
{
	(void)signum;
	g_sig = SIGINT;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/* -------------------------------------------------------------------------- */
/* Install prompt-time signal disposition                                     */
/* -------------------------------------------------------------------------- */
/*
** setup_signal_handlers
** ---------------------
** For the interactive prompt, install:
**   - SIGINT -> prompt_sigint with SA_RESTART (readline will retry syscalls)
**   - SIGQUIT -> ignored (like bash)
*/
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

/* -------------------------------------------------------------------------- */
/* Child process signal disposition                                           */
/* -------------------------------------------------------------------------- */
/*
** setup_child_signals
** -------------------
** For child processes just before execve:
**   - SIGINT/SIGQUIT -> default handling (so ^C/^\\ are delivered to the child)
*/
void	setup_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
