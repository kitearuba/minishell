/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 01:35:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 12:24:20 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*handle_parse_error(
		t_command *head, t_command *current, t_list *args)
{
	if (head)
		free_commands(head);
	if (current)
		free_commands(current);
	if (args)
		ft_lstclear(&args, free);
	return (NULL);
}

void	add_redirection(t_command *cmd, int type, char *filename, int quoted)
{
	t_redirection	*rd;
	t_redirection	*tmp;

	rd = malloc(sizeof(t_redirection));
	if (!rd)
		return ;
	ft_memset(rd, 0, sizeof(t_redirection));
	rd->type = type;
	rd->filename = ft_strdup(filename);
	if (!rd->filename)
	{
		free(rd);
		return ;
	}
	rd->quoted = quoted;
	if (!cmd->redirection)
		cmd->redirection = rd;
	else
	{
		tmp = cmd->redirection;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = rd;
	}
}

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
	int	quoted;

	(void)args;
	if (!tok->next)
	{
		ft_printf_fd(2, "Syntax error: missing filename after redirection\n");
		if (*current)
			free_commands(*current);
		*current = NULL;
		return (1);
	}
	quoted = 0;
	if (tok->type == heredoc_tok)
		quoted = tok->next->quoted;
	add_redirection(*current, tok->type, tok->next->value, quoted);
	return (0);
}

int	handle_token_type(t_token *tok, t_command **current, t_list **args)
{
	(void)current;
	if (tok->type == word || tok->type == single_quote
		|| tok->type == double_quote || tok->type == env_var)
		add_token_argument(tok, args);
	return (0);
}
