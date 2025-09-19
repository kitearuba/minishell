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
    int	st;

    if (!cmds)
        return (127);
    if (cmds->next)
    {
        execute_pipeline(cmds, bash);
        return (bash->exit_status);
    }
    if (!cmds->argv || !cmds->argv[0])
        return (127);

    /* apply redirections and PRESERVE 130 from heredoc ^C */
    st = 0;
    if (cmds->redirection)
    {
        st = apply_redirections(cmds->redirection, bash);
        if (st != 0)
        {
            if (st == 130 || bash->exit_status == 130)
                return (130);
            if (bash->exit_status == 0)
                bash->exit_status = 1;
            return (bash->exit_status);
        }
    }
    if (is_builtin(cmds->argv[0]))
        bash->exit_status = run_builtin(cmds->argv, bash);
    else
        bash->exit_status = exec_external(cmds->argv, bash);
    return (bash->exit_status);
}
