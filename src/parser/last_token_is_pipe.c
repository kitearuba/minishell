/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_token_is_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:35:48 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:23:07 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Utility: detect trailing pipe token                                        */
/* -------------------------------------------------------------------------- */

/*
** last_token_is_pipe
** ------------------
** Return 1 if the final token in the list is a PIPE, else 0.
** NULL-safe.
*/
int	last_token_is_pipe(t_token *tok)
{
	if (!tok)
		return (0);
	while (tok->next)
		tok = tok->next;
	return (tok->type == pipe_tok);
}
