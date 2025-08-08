/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/19 00:25:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_wildcard(const char *str)
{
	while (*str)
	{
		if (*str == '*')
			return (1);
		str++;
	}
	return (0);
}

static int	match_pattern(const char *pattern, const char *filename)
{
	if (*pattern == '\0' && *filename == '\0')
		return (1);
	if (*pattern == '*')
	{
		if (match_pattern(pattern + 1, filename))
			return (1);
		if (*filename && match_pattern(pattern, filename + 1))
			return (1);
	}
	if (*pattern == *filename)
		return (match_pattern(pattern + 1, filename + 1));
	return (0);
}

static char	**list_to_array(t_list *lst)
{
	int		len;
	char	**array;
	int		i;

	len = ft_lstsize(lst);
	array = malloc(sizeof(char *) * (len + 1));
	if (!array)
		return (NULL);
	i = 0;
	while (lst)
	{
		array[i] = lst->content;
		lst = lst->next;
		i++;
	}
	array[i] = NULL;
	return (array);
}

static char	**get_matches(const char *pattern)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*matches;
	char			**result;

	dir = opendir(".");
	if (!dir)
		return (NULL);
	matches = NULL;
	entry = readdir(dir);
	while (entry)
	{
		if (entry->d_name[0] != '.' && match_pattern(pattern, entry->d_name))
			ft_lstadd_back(&matches, ft_lstnew(ft_strdup(entry->d_name)));
		entry = readdir(dir);
	}
	closedir(dir);
	result = list_to_array(matches);
	ft_lstclear(&matches, free);
	return (result);
}

char	**expand_token_if_wildcard(char *token)
{
	if (!is_wildcard(token))
		return (ft_split(token, ' '));
	return (get_matches(token));
}
