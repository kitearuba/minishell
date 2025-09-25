/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:20:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:07:57 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* builtin: pwd                                                               */
/* -------------------------------------------------------------------------- */
/*
** ft_pwd
** ------
** Prints current working directory using getcwd(NULL, 0).
** On getcwd failure prints perror("pwd") and returns 1.
**
** Params:  argv : char** (ignored)
**          bash : t_bash* (sets exit_status)
** Return:  int   0 on success, 1 on error
*/
int	ft_pwd(char **argv, t_bash *bash)
{
	char	*cwd;

	(void)argv;
	cwd = getcwd(NULL, 0);
	if (!cwd)
	{
		perror("pwd");
		bash->exit_status = 1;
		return (1);
	}
	ft_putendl_fd(cwd, STDOUT_FILENO);
	free(cwd);
	bash->exit_status = 0;
	return (0);
}
