/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/29 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/18 21:45:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

void	add_redirection(t_command *cmd, int type, char *filename)
{
	t_redirection	*redirection;
	t_redirection	*temp;

	redirection = malloc(sizeof(t_redirection));
	if (!redirection)
		return ;
	ft_memset(redirection, 0, sizeof(t_redirection));
	redirection->type = type;
	redirection->filename = ft_strdup(filename);
	if (!cmd->redirection)
		cmd->redirection = redirection;
	else
	{
		temp = cmd->redirection;
		while (temp->next)
			temp = temp->next;
		temp->next = redirection;
	}
}

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

int	handle_parse_redirection(t_token *tok, t_command **current)
{
	if (tok->next)
	{
		add_redirection(*current, tok->type, tok->next->value);
		return (0);
	}
	ft_printf_fd(2,
		"Syntax error: missing filename after redirection\n");
	free_commands(*current);
	*current = NULL;
	return (1);
}
