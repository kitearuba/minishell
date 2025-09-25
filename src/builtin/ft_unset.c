/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 01:34:05 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:33:19 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Find index of KEY in env by matching "KEY=" prefix                         */
/* Return the index of KEY in env (matches "KEY=" prefix), or -1 if missing.  */
/* -------------------------------------------------------------------------- */
static int	env_index_of(char **env, const char *key)
{
	size_t	klen;
	int		i;

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

/* -------------------------------------------------------------------------- */
/* Copy all entries from src to dst, skipping index `skip`.                   */
/* Also frees src[skip] if it exists.                     .                   */
/* -------------------------------------------------------------------------- */
static void	copy_except(char **src, char **dst, int skip)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (src[i])
	{
		if (i != skip)
			dst[j++] = src[i];
		else
			free(src[i]);
		i++;
	}
	dst[j] = NULL;
}

/* -------------------------------------------------------------------------- */
/* Remove env[idx] from bash->env (by rebuilding the array without it)        */
/* -------------------------------------------------------------------------- */
/*
** env_remove_at (file-local)
** --------------------------
** Rebuilds environment without the `idx` entry.
**
** Return: int 0 on success (or idx<0), 1 on malloc failure
*/
static int	env_remove_at(t_bash *bash, int idx)
{
	char	**newenv;
	int		len;

	if (idx < 0 || !bash || !bash->env)
		return (0);
	len = 0;
	while (bash->env[len])
		len++;
	newenv = (char **)malloc(sizeof(char *) * len);
	if (!newenv)
		return (1);
	copy_except(bash->env, newenv, idx);
	free(bash->env);
	bash->env = newenv;
	return (0);
}

/* -------------------------------------------------------------------------- */
/* Unset a single key with print bash-like error on identifier validation     */
/* -------------------------------------------------------------------------- */
/*
** unset_one (file-local)
** ----------------------
** Validates KEY; prints bash-like error if invalid. Removes from env if exists.
**
** Return: int 0 on success/no-op, 1 on error
*/
static int	unset_one(t_bash *bash, const char *key)
{
	int	idx;

	if (!is_valid_identifier((char *)key))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: unset: `"
			"%s': not a valid identifier\n", key);
		return (1);
	}
	idx = env_index_of(bash->env, key);
	if (idx >= 0 && env_remove_at(bash, idx) != 0)
		return (1);
	return (0);
}

/* -------------------------------------------------------------------------- */
/* builtin: unset                                                             */
/* -------------------------------------------------------------------------- */
/*
** ft_unset
** -------
** Unsets each provided identifier. Invalid names print an error; overall
** status is 1 if any unset failed/invalid, else 0.
*** - Invalid identifiers print error to stderr and set exit status to 1.
** - Missing names are not errors.
**
** Params:  argv : char**  (argv[0]=="unset")
**          bash : t_bash*
** Return:  int   cumulative status
*/
int	ft_unset(char **argv, t_bash *bash)
{
	int	i;
	int	err;

	if (!argv || !bash)
		return (1);
	err = 0;
	i = 1;
	while (argv[i])
	{
		err |= unset_one(bash, argv[i]);
		i++;
	}
	bash->exit_status = err;
	return (err);
}
