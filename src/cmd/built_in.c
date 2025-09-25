/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_in.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:08:31 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* -------------------------------------------------------------------------- */
/* Builtin dispatcher                                                         */
/* -------------------------------------------------------------------------- */
/*
** run_builtin
** -----------
** Execute a builtin if argv[0] matches one of the supported names.
** The command runs in the current process (no fork), so state changes
** (env, cwd, exit status) take effect immediately.
**
** Notes:
** - "exit" transfers control to ft_exit(), which does not return.
** - For unknown names, this function returns 1 so the caller can try execve.
**
** Params:
**   argv : char**  - command vector (argv[0] must be non-NULL)
**   bash : t_bash* - shell state (env, last status, etc.)
**
** Returns:
**   int            - builtin-specific status (usually 0/1),
**                    or 1 if argv[0] is not a builtin.
*/
int	run_builtin(char **argv, t_bash *bash)
{
	if (ft_strcmp(argv[0], "echo") == 0)
		return (ft_echo(argv, bash));
	else if (ft_strcmp(argv[0], "cd") == 0)
		return (ft_cd(argv, bash));
	else if (ft_strcmp(argv[0], "pwd") == 0)
		return (ft_pwd(argv, bash));
	else if (ft_strcmp(argv[0], "export") == 0)
		return (ft_export(argv, bash));
	else if (ft_strcmp(argv[0], "unset") == 0)
		return (ft_unset(argv, bash));
	else if (ft_strcmp(argv[0], "env") == 0)
		return (ft_env(argv, bash));
	else if (ft_strcmp(argv[0], "exit") == 0)
		ft_exit(argv, bash);
	return (1);
}

/* -------------------------------------------------------------------------- */
/* Builtin checker                                                            */
/* -------------------------------------------------------------------------- */
/*
** is_builtin
** ----------
** Tell whether 'cmd' is the name of a supported builtin. Used by the
** executor to decide whether to run in the parent (no pipe) or fork.
**
** Params:
**   cmd : const char* - command name (may be NULL)
**
** Returns:
**   int               - 1 if builtin, 0 otherwise.
*/
int	is_builtin(const char *cmd)
{
	if (!cmd)
		return (0);
	return (!ft_strcmp(cmd, "cd")
		|| !ft_strcmp(cmd, "echo")
		|| !ft_strcmp(cmd, "pwd")
		|| !ft_strcmp(cmd, "export")
		|| !ft_strcmp(cmd, "unset")
		|| !ft_strcmp(cmd, "env")
		|| !ft_strcmp(cmd, "exit"));
}
