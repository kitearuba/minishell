/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_minishell.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 17:59:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/01 16:40:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	env_count(char **envp)
{
    int	i;

    i = 0;
    while (envp && envp[i])
        i++;
    return (i);
}

static char	**env_deep_copy(char **envp)
{
    int		n;
    int		i;
    char	**dup;

    n = env_count(envp);
    dup = (char **)malloc(sizeof(char *) * (n + 1));
    if (!dup)
        return (NULL);
    i = 0;
    while (i < n)
    {
        dup[i] = ft_strdup(envp[i]);
        if (!dup[i])
            return (free_2d_array(dup), (char **)NULL);
        i++;
    }
    dup[i] = NULL;
    return (dup);
}

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