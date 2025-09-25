/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:12:40 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Execute a program image (no return on success)                             */
/* -------------------------------------------------------------------------- */
/*
** execve_cmd
** ----------
** Resolve argv[0] against PATH and replace the current process image with it.
** If the command cannot be resolved, print "command not found" and exit 127.
** If execve() fails after a path was found, print perror and exit 126.
**
** Params:
**   argv : char**   - argument vector (argv[0] = program name)
**   env  : char**   - environment (NULL-terminated "KEY=VAL" array)
**   bash : t_bash*  - shell state (used by free_all_and_exit)
*/
void	execve_cmd(char **argv, char **env, t_bash *bash)
{
	char	*path;

	path = get_cmd_path(argv[0], env);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putendl_fd(argv[0], 2);
		free_all_and_exit(bash, 127);
	}
	execve(path, argv, env);
	perror("execve");
	free(path);
	free_all_and_exit(bash, 126);
}
