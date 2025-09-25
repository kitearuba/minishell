/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 14:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/07 06:55:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
