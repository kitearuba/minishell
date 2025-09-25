/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:32:35 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Apply redirections for a single command (current process)                  */
/* -------------------------------------------------------------------------- */
/*
** handle_redirs
** -------------
** Apply the redirection list of 'cmds' to the current process (dup2/close).
** Used for single, non-piped execution; the pipeline path applies redirs per
** child stage on its own.
**
** On error, heredoc SIGINT is propagated as 130; other errors set
** bash->exit_status to 1 when unset and return that value.
**
** Params:
**   cmds : t_command* - command node with 'redirection' set (may be NULL)
**   bash : t_bash*    - shell state (exit_status may be updated)
**
** Returns:
**   int               - 0 on success;
**                       130 on SIGINT during heredoc;
**                       >0 matching bash->exit_status for other failures.
*/
static int	handle_redirs(t_command *cmds, t_bash *bash)
{
	int	st;

	if (!cmds->redirection)
		return (0);
	st = apply_redirections(cmds->redirection, bash);
	if (st != 0)
	{
		if (st == 130 || bash->exit_status == 130)
			return (130);
		if (bash->exit_status == 0)
			bash->exit_status = 1;
		return (bash->exit_status);
	}
	return (0);
}

/* -------------------------------------------------------------------------- */
/* Run a single (non-pipeline) command                                        */
/* -------------------------------------------------------------------------- */
/*
** run_single
** ----------
** Execute one command in the current process:
** - If argv[0] is a builtin, run it directly (no fork).
** - Otherwise, exec an external command (fork + execve in helpers).
** Redirections are applied beforehand in the current process.
**
** Notes:
** - Empty argv or argv[0] returns 127 (not found), matching tester behavior.
** - bash->exit_status is always updated with the final status.
**
** Params:
**   cmds : t_command* - command to execute (argv, redirections)
**   bash : t_bash*    - shell state (env, exit_status)
**
** Returns:
**   int            - resulting exit status (also stored in bash->exit_status)
*/
static int	run_single(t_command *cmds, t_bash *bash)
{
	int	st;

	if (!cmds->argv || !cmds->argv[0])
		return (127);
	st = handle_redirs(cmds, bash);
	if (st != 0)
		return (st);
	if (is_builtin(cmds->argv[0]))
		bash->exit_status = run_builtin(cmds->argv, bash);
	else
		bash->exit_status = exec_external(cmds->argv, bash);
	return (bash->exit_status);
}

/* -------------------------------------------------------------------------- */
/* Top-level command executor (single vs pipeline)                            */
/* -------------------------------------------------------------------------- */
/*
** execute_command
** ---------------
** Entry point for executing a parsed command list:
** - If 'cmds->next' exists, run the pipeline executor (one child per stage).
** - Otherwise, run a single command path (builtins in parent, externals via 
** exec).
**
** Params:
**   cmds : t_command* - head of the command list
**   bash : t_bash*    - shell state (env, exit_status)
**
** Returns:
**   int               - final exit status (also stored in bash->exit_status)
*/
int	execute_command(t_command *cmds, t_bash *bash)
{
	if (!cmds)
		return (127);
	if (cmds->next)
	{
		execute_pipeline(cmds, bash);
		return (bash->exit_status);
	}
	return (run_single(cmds, bash));
}
