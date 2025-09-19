/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 15:59:28 by bsamy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	if (*p == *s)
		return (match_pattern(p + 1, s + 1));
	return (0);
}

void	append_token(t_token **head, t_token **tail, t_token *new)
{
	if (!*head)
		*head = new;
	else
		(*tail)->next = new;
	*tail = new;
}

void	handle_match_loop(DIR *dir, const char *pattern,
	t_token **head, int space_before)
{
	struct dirent	*entry;
	t_token			*tmp;
	t_token			*tail;
	int				first;
	int				sb;

	first = 1;
	tail = get_tail(*head);
	entry = get_next_entry(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
		{
			if (first)
				sb = space_before;
			else
				sb = 1;
			tmp = new_token(WORD, entry->d_name,
					ft_strlen(entry->d_name), 0, sb);
			if (tmp)
				append_token(head, &tail, tmp);
			first = 0;
		}
		entry = get_next_entry(dir);
	}
}

t_token	*wildcard_match(const char *pattern, int space_before)
{
	t_token	*head;
	t_token	*tail;
	DIR		*dir;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	head = NULL;
	tail = NULL;
	handle_match_loop(dir, pattern, &head, space_before);
	closedir(dir);
	return (head);
}
