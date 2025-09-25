/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_parse_redirection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:22:49 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Redirection filename collection (handles token glue across words/quotes)   */
/* -------------------------------------------------------------------------- */

/*
** is_filename_token
** -----------------
** Return 1 if token type can be glued into a filename:
**   WORD, SINGLE_QUOTE, DOUBLE_QUOTE, ENV_VAR. Else 0.
*/
static int	is_filename_token(t_token *t)
{
	if (!t)
		return (0);
	if (t->type == word || t->type == single_quote
		|| t->type == double_quote || t->type == env_var)
		return (1);
	return (0);
}

/*
** missing_redir_filename
** ----------------------
** True when a redirection is not followed by a valid filename token:
**   - no next, or next is PIPE, or another redirection.
*/
static int	missing_redir_filename(t_token *tok)
{
	if (!tok->next)
		return (1);
	if (tok->next->type == pipe_tok)
		return (1);
	if (tok->next->type >= redirect_in && tok->next->type <= heredoc_tok)
		return (1);
	return (0);
}

/*
** collect_filename
** ----------------
** Starting at 'cur' (first filename token), concatenate subsequent glued
** tokens (space_before==0) into one malloc'ed string. Consumes the linked
** tokens it merges (frees nodes/values). Returns the joined filename or NULL.
*/
static char	*collect_filename(t_token *cur)
{
	char	*name;
	char	*tmp;
	t_token	*rem;

	name = ft_strdup(cur->value);
	if (!name)
		return (NULL);
	while (cur->next && is_filename_token(cur->next)
		&& cur->next->space_before == 0)
	{
		tmp = ft_strjoin(name, cur->next->value);
		free(name);
		name = tmp;
		rem = cur->next;
		cur->next = rem->next;
		free(rem->value);
		free(rem);
	}
	return (name);
}

/*
** handle_parse_redirection
** ------------------------
** Validate and collect the redirection target (handles glued tokens),
** propagate heredoc 'quoted' flag, and call add_redirection.
** Returns 0 on success, 1 on error (and frees *current).
*/
int	handle_parse_redirection(t_token *tok, t_command **current)
{
	t_token	*cur;
	char	*name;
	int		quoted;

	if (missing_redir_filename(tok))
	{
		ft_printf_fd(2, "Syntax error: missing filename after redirection\n");
		if (*current)
			free_commands(*current);
		*current = NULL;
		return (1);
	}
	cur = tok->next;
	name = collect_filename(cur);
	if (!name)
		return (1);
	quoted = 0;
	if (tok->type == heredoc_tok)
		quoted = cur->quoted;
	add_redirection(*current, tok->type, name, quoted);
	free(name);
	return (0);
}
