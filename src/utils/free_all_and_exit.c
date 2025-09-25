/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_and_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:25:01 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Centralized cleanup + exit                                                 */
/* -------------------------------------------------------------------------- */
/*
** free_all_and_exit
** -----------------
** Frees owned resources in t_bash (env, tokens, commands) and exits with the
** provided code. Safe to call with a partially-initialized t_bash.
**
** Params:
**   bash       : t_bash* - shell state (may be NULL members)
**   exit_code  : int     - process exit code
*/
void	free_all_and_exit(t_bash *bash, int exit_code)
{
	if (bash)
	{
		if (bash->env)
			free_2d_array(bash->env);
		if (bash->tokens)
			free_tokens(bash->tokens);
		if (bash->commands)
			free_commands(bash->commands);
	}
	exit(exit_code);
}
