/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/25 12:15:30 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:24:01 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Parser: command node creation and chaining                                 */
/* -------------------------------------------------------------------------- */

/*
** new_command
** -----------
** Allocate and zero a t_command node (argv=NULL, redirection=NULL, next=NULL).
** Returns pointer or NULL on allocation failure.
*/
t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_command));
	return (cmd);
}

/*
** last_command
** ------------
** Return the last node in a t_command singly-linked list (or NULL).
*/
t_command	*last_command(t_command *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}
