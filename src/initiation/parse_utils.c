/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 12:14:59 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* filename token types allowed to be glued without space */
static int	is_filename_token(t_token *t)
{
	if (!t)
		return (0);
	if (t->type == word || t->type == single_quote
		|| t->type == double_quote || t->type == env_var)
		return (1);
	return (0);
}

/* check syntax: need a filename token after a redirection */
static int	missing_redir_filename(t_token *tok)
{
	if (!tok->next)
		return (1);
	if (tok->next->type == pipe_tok)
		return (1);
	if (tok->next->type >= redirect_in && tok->next->type <= heredoc_tok)
		return (1);
	return (0);
}

/* build filename from tok->next and following glued tokens; consumes 
 * next nodes */
static char	*collect_filename(t_token *cur)
{
	char	*name;
	char	*tmp;
	t_token	*rem;

	name = ft_strdup(cur->value);
	if (!name)
		return (NULL);
	while (cur->next && is_filename_token(cur->next)
		&& cur->next->space_before == 0)
	{
		tmp = ft_strjoin(name, cur->next->value);
		free(name);
		name = tmp;
		rem = cur->next;
		cur->next = rem->next;
		free(rem->value);
		free(rem);
	}
	return (name);
}

int	handle_parse_redirection(t_token *tok, t_command **current)
{
	t_token	*cur;
	char	*name;
	int		quoted;

	if (missing_redir_filename(tok))
	{
		ft_printf_fd(2, "Syntax error: missing filename after redirection\n");
		if (*current)
			free_commands(*current);
		*current = NULL;
		return (1);
	}
	cur = tok->next;
	name = collect_filename(cur);
	if (!name)
		return (1);
	quoted = 0;
	if (tok->type == heredoc_tok)
		quoted = cur->quoted;
	add_redirection(*current, tok->type, name, quoted);
	free(name);
	return (0);
}
