/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 19:28:30 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Basic '*' matcher, no '?', recursive, bash-like. */
int	match_pattern(const char *p, const char *s)
{
	if (!*p && !*s)
		return (1);
	if (*p == '*')
	{
		if (match_pattern(p + 1, s))
			return (1);
		if (*s && match_pattern(p, s + 1))
			return (1);
	}
	if (*p && *s && *p == *s)
		return (match_pattern(p + 1, s + 1));
	return (0);
}

static int	lexcmp(const char *a, const char *b)
{
	size_t	la;
	size_t	lb;
	size_t	n;

	la = ft_strlen(a);
	lb = ft_strlen(b);
	n = la + 1;
	if (lb + 1 > n)
		n = lb + 1;
	return (ft_strncmp(a, b, n));
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

/* Build a sorted list of matches; first keeps original space_before. */
static void	handle_match_loop(DIR *dir, const char *pattern,
		t_token **head, int space_before)
{
	struct dirent	*entry;
	int				first;
	int				show_hidden;
	t_token			*tok;
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
			tok = new_token(WORD, entry->d_name,
					ft_strlen(entry->d_name), 0);
			if (tok)
			{
				tok->space_before = sb;
				insert_sorted(head, tok);
			}
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
