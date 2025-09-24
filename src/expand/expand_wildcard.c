/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/23 22:21:11 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static t_token	*get_tail(t_token *n)
{
	while (n && n->next)
	{
		n = n->next;
	}
	return (n);
}

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
