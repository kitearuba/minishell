/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/05/20 16:43:48 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                           execute_command                                 */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Determines if the given command is a built-in and executes it, or       */
/*    otherwise runs it as an external command.                               */
/*                                                                            */
/*  Parameters:                                                               */
/*  - argv: NULL-terminated array of command arguments.                       */
/*  - bash: pointer to the bash struct containing environment and status.     */
/*                                                                            */
/*  Return:                                                                   */
/*  - Exit status of the executed command.                                    */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Calls is_builtin() to check if the command is a supported built-in.     */
/*  - Uses run_builtin() for built-ins or exec_external() for other commands. */
/*                                                                            */
/* ************************************************************************** */
int	execute_command(char **argv, t_bash *bash)
{
    int     i;

    i = 0;
	if (is_builtin(argv[i]))
		return (run_builtin(argv, bash));
	else
        return (exec_external(argv, bash));
}
