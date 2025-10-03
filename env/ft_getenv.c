/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getenv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 15:15:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:27:22 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* -------------------------------------------------------------------------- */
/* Env lookup                                                                 */
/* -------------------------------------------------------------------------- */
/*
** ft_getenv
** ---------
** Linear search over a NULL-terminated environment vector 'envp' for 'key'
** (exact match before the '='). Returns a non-owning pointer to the value
** substring (the bytes after '=') or NULL if not found.
**
** Complexity: O(N * |key|)
**
** Params:
**   envp : char**  - current environment (NULL-terminated, "KEY=VALUE")
**   key  : char*   - variable name to search (must not include '=')
**
** Returns:
**   char*          - pointer into envp[i] value (do NOT free) or NULL.
*/
char	*ft_getenv(char **envp, char *key)
{
	size_t	len;
	int		i;

	if (!key || !envp)
		return (NULL);
	len = ft_strlen(key);
	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (envp[i] + len + 1);
		i++;
	}
	return (NULL);
}
