/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/19 00:45:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	append_env_var(char ***env, const char *new_var)
{
    int		i;
    char	**new_env;

    i = 0;
    while ((*env)[i])
        i++;
    new_env = malloc(sizeof(char *) * (i + 2));
    if (!new_env)
        return (1);
    i = -1;
    while ((*env)[++i])
        new_env[i] = ft_strdup((*env)[i]);
    new_env[i] = ft_strdup(new_var);
    new_env[i + 1] = NULL;
    free_2d_array(*env);
    *env = new_env;
    return (0);
}
