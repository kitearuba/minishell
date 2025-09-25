/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:07:44 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Append one "KEY=VAL" entry to env (deep copy)                              */
/* -------------------------------------------------------------------------- */
/*
** append_env_var
** --------------
** Reallocates env vector +1, duplicates strings, frees old env on success.
**
** Params:  env     : char*** environment vector ref
**          new_var : const char* "KEY=VAL"
** Return:  int     0 on success, 1 on alloc failure
*/
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
