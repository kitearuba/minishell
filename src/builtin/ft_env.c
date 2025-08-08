/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 09:15:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/08 03:18:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
