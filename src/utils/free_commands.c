/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/19 00:20:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	free_argv(char **argv)
{
	int	i;

	i = 0;
	while (argv[i])
		free(argv[i++]);
	free(argv);
}

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
