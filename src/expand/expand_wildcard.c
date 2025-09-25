/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:20:04 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Wildcard entry point: replace unquoted WORDs that contain '*'              */
/* with a sorted list of filename tokens (or leave literal when no match).    */
/* -------------------------------------------------------------------------- */

/*
** get_tail
** --------
** Return the last node of a token list (or NULL).
*/
static t_token	*get_tail(t_token *n)
{
	while (n && n->next)
	{
		n = n->next;
	}
	return (n);
}

/*
** replace_with_matches
** --------------------
** For the current WORD (*curr) containing '*':
**   - Build the match list via wildcard_match (sorted, with space_before set).
**   - Splice matches into the list in place of *curr.
**   - Free the original *curr node.
** Returns the last token of the inserted block (tail) to resume iteration.
** If no matches found, leaves the list unchanged and returns prev.
*/
static t_token	*replace_with_matches(t_token **tokens, t_token *prev,
		t_token **curr)
{
	t_token	*matches;
	t_token	*tail;

	matches = wildcard_match((*curr)->value, (*curr)->space_before);
	if (!matches)
		return (prev);
	tail = get_tail(matches);
	tail->next = (*curr)->next;
	if (prev)
		prev->next = matches;
	else
		*tokens = matches;
	free((*curr)->value);
	free(*curr);
	*curr = tail->next;
	return (tail);
}

/*
** expand_wildcards
** ----------------
** Iterate over the token list:
**   - If token is WORD, unquoted, and contains '*',
**       expand via replace_with_matches.
**   - Otherwise move to next token.
*/
void	expand_wildcards(t_token **tokens)
{
	t_token	*prev;
	t_token	*curr;

	prev = NULL;
	curr = *tokens;
	while (curr)
	{
		if (curr->type == word
			&& curr->quoted == 0 && ft_strchr(curr->value, '*'))
		{
			prev = replace_with_matches(tokens, prev, &curr);
			continue ;
		}
		prev = curr;
		curr = curr->next;
	}
}
