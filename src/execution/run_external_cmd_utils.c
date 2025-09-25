/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_external_cmd_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsamy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:11:01 by bsamy             #+#    #+#             */
/*   Updated: 2025/09/25 15:18:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* Ignore SIGINT/SIGQUIT in the parent around fork/exec windows. */
void	parent_ignore_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

/* Restore prompt-time signal handlers in the parent after child completion. */
void	parent_restore_prompt_signals(void)
{
	setup_signal_handlers();
}
