/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:30:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:05:23 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Validate shell identifier: [A-Za-z_][A-Za-z0-9_]*                          */
/* -------------------------------------------------------------------------- */
/*
** is_valid_identifier
** -------------------
** Returns 1 if `s` is a valid identifier for export/unset, else 0.
**
** Params:  s : candidate string
** Return:  int (1 valid, 0 invalid)
*/
int	is_valid_identifier(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	if (!(ft_isalpha(*s) || *s == '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!(ft_isalnum(s[i]) || s[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/* -------------------------------------------------------------------------- */
/* Deep-copy a NULL-terminated environment vector                             */
/* -------------------------------------------------------------------------- */
/*
** copy_env
** --------
** Allocates and duplicates each "KEY=VAL" entry from `env`.
**
** Params:  env : char** (NULL-terminated)
** Return:  char** newly allocated copy (or NULL on alloc failure)
*/
char	**copy_env(char **env)
{
	int		count;
	char	**copy;
	int		i;

	count = 0;
	while (env[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
