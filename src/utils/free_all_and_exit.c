/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_all_and_exit.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/19 00:20:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	free_all_and_exit(t_bash *bash, int exit_code)
{
    if (bash)
    {
        if (bash->env)
            free_2d_array(bash->env);
        if (bash->tokens)
            free_tokens(bash->tokens);
        if (bash->commands)
            free_commands(bash->commands);
    }
    exit(exit_code);
}