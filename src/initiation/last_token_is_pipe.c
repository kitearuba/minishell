/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   last_token_is_pipe.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 22:35:48 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/18 23:13:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	last_token_is_pipe(t_token *tok)
{
	if (!tok)
		return (0);
	while (tok->next)
		tok = tok->next;
	return (tok->type == PIPE);
}
