/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:59:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:10:47 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** env_count
** ---------
** Count the number of strings in a NULL-terminated envp array.
** Safe when envp == NULL (returns 0).
*/
static int	env_count(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

/*
** free_partial
** ------------
** Helper for deep-copy failures: frees the first 'count' entries and the array.
*/
static void	free_partial(char **dup, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(dup[i]);
		i++;
	}
	free(dup);
}

/*
** env_deep_copy
** -------------
** Deep copy of the host environment (char **envp) into a freshly allocated
** NULL-terminated array owned by minishell. Returns NULL on allocation error.
*/
static char	**env_deep_copy(char **envp)
{
	int		n;
	int		i;
	char	**dup;

	n = env_count(envp);
	dup = malloc(sizeof(char *) * (n + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (i < n)
	{
		dup[i] = ft_strdup(envp[i]);
		if (!dup[i])
		{
			free_partial(dup, i);
			return (NULL);
		}
		i++;
	}
	dup[i] = NULL;
	return (dup);
}

/*
** init_minishell
** --------------
** Zero the main 't_bash' state, duplicate environment, and set defaults.
** Returns 0 on success, 1 on allocation failure.
*/
int	init_minishell(t_bash *bash, char **envp)
{
	ft_memset(bash, 0, sizeof(*bash));
	bash->env = env_deep_copy(envp);
	if (!bash->env)
		return (1);
	bash->exit_status = 0;
	bash->tokens = NULL;
	bash->commands = NULL;
	return (0);
}
