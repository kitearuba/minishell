/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:31:01 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* -------------------------------------------------------------------------- */
/* Wildcard helpers: generate a sorted token list of matches for a pattern.   */
/* Hidden files are included only when the pattern begins with '.'.           */
/* -------------------------------------------------------------------------- */

/*
** lexcmp
** ------
** ASCII lexicographic compare wrapper (uses ft_strcmp).
*/
static int	lexcmp(const char *a, const char *b)
{
	return (ft_strcmp(a, b));
}

/*
** insert_sorted
** -------------
** Insert 'node' into the singly-linked token list pointed by *head
** preserving ASCII lexicographic order by token->value.
*/
static void	insert_sorted(t_token **head, t_token *node)
{
	t_token	*prev;
	t_token	*cur;

	if (!*head || lexcmp(node->value, (*head)->value) < 0)
	{
		node->next = *head;
		*head = node;
		return ;
	}
	prev = *head;
	cur = (*head)->next;
	while (cur && lexcmp(node->value, cur->value) >= 0)
	{
		prev = cur;
		cur = cur->next;
	}
	prev->next = node;
	node->next = cur;
}

/*
** append_name
** -----------
** Create a WORD token for 'name', set space_before to 'sb', and insert it
** into the list in sorted order. No-op on allocation failure.
*/
static void	append_name(t_token **head, const char *name, int sb)
{
	t_token	*tok;

	tok = new_token(word, name, ft_strlen(name), 0);
	if (!tok)
		return ;
	tok->space_before = sb;
	insert_sorted(head, tok);
}

/*
** handle_match_loop
** -----------------
** Iterate directory entries from 'dir', and for each entry that:
**   - is not hidden unless pattern starts with '.'
**   - matches the glob 'pattern' (match_pattern)
** append a token with correct space_before:
**   - first match keeps original space_before
**   - subsequent matches use space_before = 1
*/
static void	handle_match_loop(DIR *dir, const char *pattern,
		t_token **head, int space_before)
{
	struct dirent	*entry;
	int				first;
	int				show_hidden;
	int				sb;

	first = 1;
	show_hidden = 0;
	if (pattern[0] == '.')
		show_hidden = 1;
	entry = readdir(dir);
	while (entry)
	{
		if ((show_hidden || entry->d_name[0] != '.')
			&& match_pattern(pattern, entry->d_name))
		{
			sb = space_before;
			if (!first)
				sb = 1;
			append_name(head, entry->d_name, sb);
			first = 0;
		}
		entry = readdir(dir);
	}
}

/*
** wildcard_match
** --------------
** Return a newly-allocated, sorted list of WORD tokens matching 'pattern'.
** If no matches are found, return a single WORD token containing the literal
** pattern (so the caller can keep it untouched).
** space_before the first emitted token is set to the caller-provided flag.
*/
t_token	*wildcard_match(const char *pattern, int space_before)
{
	t_token	*head;
	t_token	*fallback;
	DIR		*dir;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	head = NULL;
	handle_match_loop(dir, pattern, &head, space_before);
	closedir(dir);
	if (head)
		return (head);
	fallback = new_token(word, pattern, ft_strlen(pattern), 0);
	if (fallback)
		fallback->space_before = space_before;
	return (fallback);
}
