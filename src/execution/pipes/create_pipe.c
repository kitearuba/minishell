/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:34:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:12:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Thin wrapper over pipe(2) with project-wide error handling                 */
/* -------------------------------------------------------------------------- */
/*
** create_pipe
** -----------
** Call pipe(2) and, on failure, exit the shell with EXIT_FAILURE after cleanup.
**
** Params:
**   pipefd : int*    - array of two ints; pipefd[0] is read end, pipefd[1] write
**   bash   : t_bash* - shell state for unified cleanup on fatal error
*/
void	create_pipe(int *pipefd, t_bash *bash)
{
	if (pipe(pipefd) == -1)
		free_all_and_exit(bash, EXIT_FAILURE);
}
