/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 09:15:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:06:30 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* builtin: env                                                               */
/* -------------------------------------------------------------------------- */
/*
** ft_env
** ------
** Prints environment entries that contain '=' exactly as "KEY=VAL".
**
** Params:  argv : char** (ignored)
**          bash : t_bash*
** Return:  int  0
*/
int	ft_env(char **argv, t_bash *bash)
{
	int	i;

	(void)argv;
	i = 0;
	while (bash->env && bash->env[i])
	{
		if (ft_strchr(bash->env[i], '='))
			ft_putendl_fd(bash->env[i], STDOUT_FILENO);
		i++;
	}
	bash->exit_status = 0;
	return (0);
}
