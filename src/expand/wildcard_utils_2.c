/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/23 22:26:11 by chrrodri         ###   ########.fr       */
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
