/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/18 06:10:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 13:43:46 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static int	env_replace(t_bash *bash, int idx, char *entry)
{
	if (idx < 0 || !entry)
		return (1);
	free(bash->env[idx]);
	bash->env[idx] = entry;
	return (0);
}

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
