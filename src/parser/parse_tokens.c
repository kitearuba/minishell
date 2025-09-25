/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:29:07 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Top-level parser: tokens -> command list (argv + redirections + pipes)     */
/* -------------------------------------------------------------------------- */

/*
** begin_command_if_needed
** -----------------------
** When *current is NULL, run early checks and allocate a new command.
** On error, clears *args and returns 1; else 0.
*/
static int	begin_command_if_needed(t_token *tok, t_command **current,
		t_list **args)
{
	if (!*current)
	{
		if (check_initial_errors(tok))
		{
			ft_lstclear(args, free);
			return (1);
		}
		*current = new_command();
		if (!*current)
		{
			ft_lstclear(args, free);
			return (1);
		}
	}
	return (0);
}

/*
** process_token
** -------------
** Consume one token and update parse state:
**   - redirection: validate filename, attach, and advance by two tokens
**     (returns 2 to signal caller to 'continue')
**   - word-like: append into args
**   - pipe: finalize current command and keep parsing
** Returns 0 on success, 1 on error, i
**    or 2 to indicate "skip advance already done".
*/
static int	process_token(t_token **ptok, t_command **head,
		t_command **current, t_list **args)
{
	t_token	*tok;

	tok = *ptok;
	if (tok->type >= redirect_in && tok->type <= heredoc_tok)
	{
		if (handle_parse_redirection(tok, current))
		{
			ft_lstclear(args, free);
			return (1);
		}
		if (!tok->next)
		{
			ft_lstclear(args, free);
			return (1);
		}
		*ptok = tok->next->next;
		return (2);
	}
	if (handle_token_type(tok, current, args))
		return (1);
	if (tok->type == pipe_tok)
		finalize_cmd(head, current, args);
	*ptok = tok->next;
	return (0);
}

/*
** parse_loop
** ----------
** Main loop over tokens: ensure a command exists, check '||', then
** delegate to process_token. Returns 0 on success, 1 on error.
*/
static int	parse_loop(t_command **head, t_command **current,
		t_list **args, t_token *tok)
{
	int	st;

	while (tok)
	{
		if (begin_command_if_needed(tok, current, args))
			return (1);
		if (check_consecutive_pipes(tok, current))
		{
			ft_lstclear(args, free);
			return (1);
		}
		st = process_token(&tok, head, current, args);
		if (st == 1)
			return (1);
		if (st == 2)
			continue ;
	}
	return (0);
}

/*
** check_parse_errors
** ------------------
** Final validation after the loop:
**   - empty input -> NULL
**   - finalize pending command
**   - trailing pipe / orphan redirection -> errors
**   - no head -> error "no command given"
** Returns head or NULL.
*/
static t_command	*check_parse_errors(t_command *head,
		t_command *current, t_list *args, t_token *tokens)
{
	if (!tokens)
		return (NULL);
	if (current)
		finalize_cmd(&head, &current, &args);
	if (last_token_is_pipe(tokens))
	{
		ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
		free_commands(head);
		return (NULL);
	}
	if (check_commandless_redirection(head))
		return (NULL);
	if (!head)
	{
		ft_printf_fd(2, "Syntax error: no command given\n");
		return (NULL);
	}
	return (head);
}

/*
** parse_tokens
** ------------
** Public API. Build a pipeline list from the token stream.
** On syntax error prints message(s) and returns NULL.
*/
t_command	*parse_tokens(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_list		*args;

	head = NULL;
	current = NULL;
	args = NULL;
	if (tokens && tokens->type == pipe_tok)
	{
		ft_printf_fd(2, "Syntax error: unexpected token `|'\n");
		return (NULL);
	}
	if (parse_loop(&head, &current, &args, tokens))
		return (handle_parse_error(head, current, args));
	return (check_parse_errors(head, current, args, tokens));
}
