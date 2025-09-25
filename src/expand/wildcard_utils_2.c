/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/23 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:21:33 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** match_pattern
** -------------
** Simple glob matcher supporting '*' only:
**   '*' matches any (possibly empty) substring
**   other characters match literally
** Returns 1 on match, 0 otherwise.
*/
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
