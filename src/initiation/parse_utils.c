/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 11:07:44 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	return (cmd);
}

t_command	*last_command(t_command *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

int	handle_parse_redirection(t_token *tok, t_command **current)
{
	t_token	*cur;
	t_token	*rem;
	char	*name;
	char	*tmp;
	int		quoted;

	if (!tok->next || tok->next->type == pipe_tok
		|| (tok->next->type >= redirect_in && tok->next->type <= heredoc_tok))
	{
		ft_printf_fd(2, "Syntax error: missing filename after redirection\n");
		if (*current)
			free_commands(*current);
		*current = NULL;
		return (1);
	}
	cur = tok->next;
	name = ft_strdup(cur->value);
	if (!name)
		return (1);
	while (cur->next
		&& (cur->next->type == word || cur->next->type == single_quote
			|| cur->next->type == double_quote || cur->next->type == env_var)
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
	quoted = (tok->type == heredoc_tok) ? tok->next->quoted : 0;
	add_redirection(*current, tok->type, name, quoted);
	free(name);
	return (0);
}
