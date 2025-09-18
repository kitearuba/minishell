/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/07 03:00:00 by chrrodri         ###   ########.fr       */
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

static int	env_set(t_bash *bash, const char *key, const char *val)
{
	int		i;
	int		idx;
	char	*entry;
	size_t	len;

	len = ft_strlen(key) + 1 + (val ? ft_strlen(val) : 0) + 1;
	entry = (char *)malloc(len);
	if (!entry)
		return (1);
	ft_strlcpy(entry, key, len);
	ft_strlcat(entry, "=", len);
	if (val)
		ft_strlcat(entry, val, len);
	idx = env_find_index(bash->env, key);
	if (idx >= 0)
	{
		free(bash->env[idx]);
		bash->env[idx] = entry;
		return (0);
	}
	i = 0;
	while (bash->env[i])
		i++;
	{
		char	**newenv;

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
		newenv[i++] = entry;
		newenv[i] = NULL;
		free(bash->env);
		bash->env = newenv;
	}
	return (0);
}

int	ft_cd(char **argv, t_bash *bash)
{
	char	oldpwd[PATH_MAX];
	char	newpwd[PATH_MAX];
	char	*dest;

	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	if (!argv[1])
	{
		dest = ft_getenv(bash->env, "HOME");
		if (!dest || !*dest)
		{
			ft_putendl_fd("minishell: cd: HOME not set", STDERR_FILENO);
			bash->exit_status = 1;
			return (1);
		}
	}
	else
		dest = argv[1];
	if (chdir(dest) == -1)
	{
		perror("minishell: cd");
		bash->exit_status = 1;
		return (1);
	}
	if (!getcwd(newpwd, sizeof(newpwd)))
		newpwd[0] = '\0';
	env_set(bash, "OLDPWD", oldpwd);
	env_set(bash, "PWD", newpwd);
	bash->exit_status = 0;
	return (0);
}