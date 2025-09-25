/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_checks.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/01 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:23:50 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Parser: syntax checks for pipes and orphan redirections                    */
/* -------------------------------------------------------------------------- */

/*
** check_leading_pipe
** ------------------
** If first token is a pipe, print error, free partial trees, and return 1.
** Otherwise return 0.
*/
int	check_leading_pipe(t_token *tokens, t_command *head, t_command *current)
{
	if (tokens && tokens->type == pipe_tok)
	{
		ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
		if (head)
			free_commands(head);
		if (current)
			free_commands(current);
		return (1);
	}
	return (0);
}

/*
** check_trailing_pipe
** -------------------
** If last token is a pipe, print error, free partial trees, and return 1.
** Otherwise return 0.
*/
int	check_trailing_pipe(t_token *tokens, t_command *head, t_command *current)
{
	if (last_token_is_pipe(tokens))
	{
		ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
		if (head)
			free_commands(head);
		if (current)
			free_commands(current);
		return (1);
	}
	return (0);
}

/*
** check_commandless_redirection
** -----------------------------
** Detect a command node that has only redirections and no argv.
** On error prints message, frees whole chain, and returns 1. Else 0.
*/
int	check_commandless_redirection(t_command *head)
{
	t_command	*cmd;

	cmd = head;
	while (cmd)
	{
		if (!cmd->argv && cmd->redirection)
		{
			ft_printf_fd(2,
				"Syntax error: unexpected redirection with no command\n");
			free_commands(head);
			return (1);
		}
		cmd = cmd->next;
	}
	return (0);
}

/*
** check_initial_errors
** --------------------
** Quick early checks before starting a command:
**   - NULL ok
**   - leading pipe -> error (1)
*/
int	check_initial_errors(t_token *tok)
{
	if (!tok)
		return (0);
	if (tok->type == pipe_tok)
	{
		ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
		return (1);
	}
	return (0);
}

/*
** check_consecutive_pipes
** -----------------------
** Detect '||' and report a syntax error. Frees *current and sets it NULL.
** Returns 1 on error, 0 otherwise.
*/
int	check_consecutive_pipes(t_token *tok, t_command **current)
{
	if (tok->type == pipe_tok && tok->next
		&& tok->next->type == pipe_tok)
	{
		ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
		free_commands(*current);
		*current = NULL;
		return (1);
	}
	return (0);
}
