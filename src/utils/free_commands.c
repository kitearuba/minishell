/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:25:17 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Free argv array                                                            */
/* -------------------------------------------------------------------------- */
/*
** free_argv (file-local)
** ----------------------
** Frees NULL-terminated argv array.
*/
static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

/* -------------------------------------------------------------------------- */
/* Free redirection list                                                      */
/* -------------------------------------------------------------------------- */
/*
** free_redirections (file-local)
** ------------------------------
** Frees a linked list of t_redirection (including each filename).
*/
static void	free_redirections(t_redirection *redir)
{
	t_redirection	*next;

	while (redir)
	{
		next = redir->next;
		free(redir->filename);
		free(redir);
		redir = next;
	}
}

/* -------------------------------------------------------------------------- */
/* Free command list                                                          */
/* -------------------------------------------------------------------------- */
/*
** free_commands
** -------------
** Frees a linked list of t_command, including argv and redirection lists for
** each node.
**
** Params:
**   cmd : t_command* - head of the command list (may be NULL)
*/
void	free_commands(t_command *cmd)
{
	t_command	*next;

	while (cmd)
	{
		next = cmd->next;
		if (cmd->argv)
			free_argv(cmd->argv);
		free_redirections(cmd->redirection);
		free(cmd);
		cmd = next;
	}
}
