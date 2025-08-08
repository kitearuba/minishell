/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_commands.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/05/20 16:43:48 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_command(t_command *cmds, t_bash *bash)
{
	if (!cmds)
		return (127);
	if (cmds->next)
	{
		execute_pipeline(cmds, bash);
		return (bash->exit_status);
	}
	if (!cmds->argv || !cmds->argv[0])
		return (127);
	if (apply_redirections(cmds->redirection, bash))
		return (1);
	if (is_builtin(cmds->argv[0]))
		bash->exit_status = run_builtin(cmds->argv, bash);
	else
		bash->exit_status = exec_external(cmds->argv, bash);
	return (bash->exit_status);
}
