/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 01:35:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 15:05:40 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	add_token_argument(t_token *tok, t_list **args)
{
	t_list	*last;
	char	*old;
	char	*joined;
	char	*dup;

	if (*args && tok->space_before == 0)
	{
		last = ft_lstlast(*args);
		old = (char *)last->content;
		joined = ft_strjoin(old, tok->value);
		free(old);
		last->content = joined;
		return ;
	}
	dup = ft_strdup(tok->value);
	ft_lstadd_back(args, ft_lstnew(dup));
}

int	handle_redirection(t_token *tok, t_command **current, t_list **args)
{
	(void)args;
	if (tok->next)
	{
		add_redirection(
			*current,
			tok->type,
			tok->next->value,
			tok->next->quoted);
		return (0);
	}
	ft_printf_fd(2, "Syntax error: missing filename after redirection\n");
	if (*current)
		free_commands(*current);
	*current = NULL;
	return (1);
}

int	handle_token_type(t_token *tok, t_command **current, t_list **args)
{
	(void)current;
	if (tok->type == WORD || tok->type == SINGLE_QUOTE
		|| tok->type == DOUBLE_QUOTE || tok->type == ENV_VAR)
		add_token_argument(tok, args);
	return (0);
}
