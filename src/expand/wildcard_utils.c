/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/23 22:39:32 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	lexcmp(const char *a, const char *b)
{
	return (ft_strcmp(a, b));
}

/* Insert node into list keeping ASCII-lexicographic order. */
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

/* Create token for name, set space_before, and insert sorted. */
static void	append_name(t_token **head, const char *name, int sb)
{
	t_token	*tok;

	tok = new_token(WORD, name, ft_strlen(name), 0);
	if (!tok)
		return ;
	tok->space_before = sb;
	insert_sorted(head, tok);
}

/* Build a sorted list of matches; first keeps original space_before. */
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

/* Public API: expand; if no match, return literal pattern as a WORD token. */
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
	fallback = new_token(WORD, pattern, ft_strlen(pattern), 0);
	if (fallback)
		fallback->space_before = space_before;
	return (fallback);
}
