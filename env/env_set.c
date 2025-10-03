/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 06:10:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:11:29 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* -------------------------------------------------------------------------- */
/* Env mutation (replace or append "KEY=VALUE" entries)                       */
/* -------------------------------------------------------------------------- */
/*
** Contract:
** - bash->env is a NULL-terminated array fully owned by the shell.
** - env_set() builds "KEY=VALUE" and either replaces an existing entry
**   (exact "KEY=" match) or reallocates the vector and appends.
** - On success returns 0; on allocation/argument error returns 1.
**
** Notes:
** - Passing val == NULL results in "KEY=" (empty value), not a deletion.
** - Returned pointers are owned by the env vector; callers should NOT free
**   env entries directly (except inside these helpers).
*/

/*
** env_find_index
** --------------
** Find the index of 'key' within env ("KEY=" exact match).
** Returns the index (>=0) or -1 if not present.
**
** Params:
**   env : char**        - environment vector
**   key : const char*   - variable name (no '=')
*/
static int	env_find_index(char **env, const char *key)
{
	int		i;
	size_t	klen;

	if (!env || !key)
		return (-1);
	klen = ft_strlen(key);
	i = 0;
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, klen) && env[i][klen] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/*
** env_make_entry
** --------------
** Build a freshly-allocated "KEY=VAL" C-string. If 'val' is NULL, produce
** "KEY=" (empty value).
**
** Params:
**   key : const char*   - variable name (required, non-empty)
**   val : const char*   - value (may be NULL)
**
** Returns:
**   char*               - malloc'ed "KEY=VAL" or NULL on error.
*/
static char	*env_make_entry(const char *key, const char *val)
{
	char	*entry;
	size_t	key_len;
	size_t	val_len;
	size_t	len;

	if (!key || !*key)
		return (NULL);
	key_len = ft_strlen(key);
	val_len = 0;
	if (val)
		val_len = ft_strlen(val);
	len = key_len + 1 + val_len + 1;
	entry = (char *)malloc(len);
	if (!entry)
		return (NULL);
	ft_strlcpy(entry, key, len);
	ft_strlcat(entry, "=", len);
	if (val)
		ft_strlcat(entry, val, len);
	return (entry);
}

/*
** env_replace
** -----------
** Replace env[idx] with 'entry'. Frees the previous entry.
** Returns 0 on success, 1 on invalid idx or NULL entry.
**
** Params:
**   bash  : t_bash*  - shell state (owns env)
**   idx   : int      - index to replace (>=0)
**   entry : char*    - malloc'ed "KEY=VAL" to take ownership of
*/
static int	env_replace(t_bash *bash, int idx, char *entry)
{
	if (idx < 0 || !entry)
		return (1);
	free(bash->env[idx]);
	bash->env[idx] = entry;
	return (0);
}

/*
** env_append
** ----------
** Reallocate env to add a new entry at the end. Takes ownership of 'entry'.
** Returns 0 on success, 1 on allocation failure (and frees 'entry').
**
** Params:
**   bash  : t_bash*  - shell state (owns env)
**   entry : char*    - malloc'ed "KEY=VAL" to append
*/
static int	env_append(t_bash *bash, char *entry)
{
	int		i;
	char	**newenv;

	if (!entry)
		return (1);
	i = 0;
	while (bash->env[i])
		i++;
	newenv = (char **)malloc(sizeof(char *) * (i + 2));
	if (!newenv)
	{
		free(entry);
		return (1);
	}
	i = 0;
	while (bash->env[i])
	{
		newenv[i] = bash->env[i];
		i++;
	}
	newenv[i] = entry;
	newenv[i + 1] = NULL;
	free(bash->env);
	bash->env = newenv;
	return (0);
}

/*
** env_set
** -------
** Public API. Create/replace "KEY=VAL" in bash->env.
**   - If KEY exists, replace in place.
**   - If KEY doesn't exist, append (reallocates env vector).
**
** Returns:
**   0 on success; 1 on allocation/argument error.
**
** Params:
**   bash : t_bash*        - shell state
**   key  : const char*    - variable name (required, non-empty)
**   val  : const char*    - value (may be NULL -> empty value)
*/
int	env_set(t_bash *bash, const char *key, const char *val)
{
	int		idx;
	char	*entry;

	entry = env_make_entry(key, val);
	if (!entry)
		return (1);
	idx = env_find_index(bash->env, key);
	if (idx >= 0)
		return (env_replace(bash, idx, entry));
	return (env_append(bash, entry));
}
