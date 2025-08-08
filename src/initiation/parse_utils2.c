/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 01:35:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/07 01:35:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_token_argument(t_token *tok, t_list **args)
{
	char	*dup;

	dup = ft_strdup(tok->value);
	ft_lstadd_back(args, ft_lstnew(dup));
}

int	handle_redirection(t_token *tok, t_command **current, t_list **args)
{
	if (handle_parse_redirection(tok, current))
	{
		ft_lstclear(args, free);
		return (1);
	}
	return (0);
}

int	handle_token_type(t_token *tok, t_command **current, t_list **args)
{
	if (tok->type == WORD || tok->type == SINGLE_QUOTE
		|| tok->type == DOUBLE_QUOTE || tok->type == ENV_VAR)
	{
		add_token_argument(tok, args);
	}
	else if (tok->type >= REDIRECT_IN && tok->type <= HEREDOC)
	{
		if (handle_redirection(tok, current, args))
			return (1);
	}
	return (0);
}
