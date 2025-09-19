/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_wildcard.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 19:23:28 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	replace_with_matches(t_token **tokens, t_token *prev, t_token **curr)
{
	t_token	*matches;
	t_token	*tail;

	matches = wildcard_match(
			(*curr)->value,
			(*curr)->space_before);
	if (!matches)
		return ;
	tail = matches;
	while (tail->next)
		tail = tail->next;
	tail->next = (*curr)->next;
	if (prev)
		prev->next = matches;
	else
		*tokens = matches;
	free((*curr)->value);
	free(*curr);
	*curr = tail->next;
}

void	expand_wildcards(t_token **tokens)
{
	t_token	*prev;
	t_token	*curr;

	prev = NULL;
	curr = *tokens;

	while (curr)
	{
		if (curr->type == WORD && curr->quoted == 0
			&& ft_strchr(curr->value, '*'))
		{
			replace_with_matches(tokens, prev, &curr);
			continue ;
		}
		prev = curr;
		curr = curr->next;
	}
}
