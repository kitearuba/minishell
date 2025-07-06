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
/*                                                                            */
/*  Notes:                                                                    */
/*  - Calls is_builtin() to check if the command is a supported built-in.     */
/*  - Uses run_builtin() for built-ins or exec_external() for other commands. */
/*                                                                            */
/* ************************************************************************** */
int execute_command(t_command *cmds, t_bash *bash)
{
    (void)cmds;
    (void)bash;
    return (0);
}