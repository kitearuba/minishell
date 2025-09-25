/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/25 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 16:27:22 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
