/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yourlogin <yourlogin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 04:00:00 by yourlogin         #+#    #+#             */
/*   Updated: 2025/08/07 04:00:00 by yourlogin        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

int	ft_unset(char **argv, t_bash *bash)
{
	int	i;
	int	err;
	int	idx;

	if (!argv || !bash)
		return (1);
	err = 0;
	i = 1;
	while (argv[i])
	{
		if (!is_valid_identifier(argv[i]))
		{
			ft_printf_fd(STDERR_FILENO,
				"minishell: unset: `%s': not a valid identifier\n", argv[i]);
			err = 1;
		}
		else
		{
			idx = env_index_of(bash->env, argv[i]);
			if (idx >= 0 && env_remove_at(bash, idx) != 0)
				err = 1;
		}
		i++;
	}
	bash->exit_status = err;
	return (err);
}