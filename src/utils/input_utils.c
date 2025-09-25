/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 15:18:38 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:44:48 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Return 1 if the string is NULL or contains only spaces/tabs                */
/* -------------------------------------------------------------------------- */
/*
** is_only_spaces
** --------------
** Treats NULL as "empty". Iterates once over the string and returns 1 if every
** character is either ' ' (space) or '\t' (tab); otherwise returns 0.
**
** Notes:
**  - Does NOT consider other whitespace (e.g., '\n', '\r', '\v', '\f').
**  - Used to skip no-op input lines at the prompt.
**
** Params:  s : const char* (may be NULL)
** Return:  int (1 if NULL or only spaces/tabs, else 0)
** Big-O:   O(n) over strlen(s)
*/
int	is_only_spaces(const char *s)
{
	int	i;

	if (!s)
		return (1);
	i = 0;
	while (s[i])
	{
		if (s[i] != ' ' && s[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}
