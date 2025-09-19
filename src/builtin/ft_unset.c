/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 01:34:05 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 12:32:34 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 04:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 13:40:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*
** env_index_of:
**   Return the index of KEY=... inside env (or -1 if not present).
*/
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

/*
** env_remove_at:
**   Remove env[idx] from bash->env without duplicating untouched entries.
**   Keeps ownership of existing strings; frees only the removed one.
*/
static int	env_remove_at(t_bash *bash, int idx)
{
	char	**newenv;
	int		len;
	int		i;
	int		j;

	if (idx < 0)
		return (0);
	len = 0;
	while (bash->env[len])
		len++;
	newenv = (char **)malloc(sizeof(char *) * len);
	if (!newenv)
		return (1);
	i = 0;
	j = 0;
	while (bash->env[i])
	{
		if (i != idx)
			newenv[j++] = bash->env[i];
		else
			free(bash->env[i]);
		i++;
	}
	newenv[j] = NULL;
	free(bash->env);
	bash->env = newenv;
	return (0);
}

/*
** unset_one:
**   Validate identifier; if present remove it. Returns 0 on success, 1 on error.
*/
static int	unset_one(t_bash *bash, const char *key)
{
	int	idx;

	if (!is_valid_identifier((char *)key))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: unset: `%s': not a valid identifier\n", key);
		return (1);
	}
	idx = env_index_of(bash->env, key);
	if (idx >= 0 && env_remove_at(bash, idx) != 0)
		return (1);
	return (0);
}

/*
** ft_unset:
**   Loop over argv[1..]; unset each valid name. Exit status is OR of errors.
*/
int	ft_unset(char **argv, t_bash *bash)
{
	int	i;
	int	err;

	if (!argv || !bash)
		return (1);
	i = 1;
	err = 0;
	while (argv[i])
	{
		if (argv[i][0] != '\0')
			err |= unset_one(bash, argv[i]);
		i++;
	}
	bash->exit_status = err;
	return (err);
}
