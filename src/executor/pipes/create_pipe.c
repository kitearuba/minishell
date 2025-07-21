/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 21:34:32 by chrrodri          #+#    #+#             */
/*   Updated: 2024/11/20 21:36:05 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

/**
 * create_pipe - Creates a pipe for inter-process communication.
 * @pipefd: Array to store the file descriptors for the pipe ends.
 * @bash: Pointer to t_bash structure (optional, for exiting cleanly).
 */
void	create_pipe(int *pipefd, t_bash *bash)
{
	if (pipe(pipefd) == -1)
		free_all_and_exit(bash, EXIT_FAILURE);
}