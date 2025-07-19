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

#include "../../include/pipex.h"

/* ************************************************************************** */
/*                              Pipe Creation                                 */
/* ************************************************************************** */

/**
 * create_pipe - Creates a pipe for inter-process communication.
 * @pipefd: Array to store the file descriptors for the pipe ends.
 * @pipex: Pointer to the pipex structure for resource cleanup on error.
 * Description:
 * - Creates a pipe and stores the file descriptors in `pipefd`.
 * - Exits with an error message if the pipe creation fails.
 */
void	create_pipe(int *pipefd, t_pipex *pipex)
{
	if (pipe(pipefd) == -1)
		free_resources_on_error(pipex, "Error creating pipe", 1);
}
