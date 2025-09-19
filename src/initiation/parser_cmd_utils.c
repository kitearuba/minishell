/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:10:35 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 16:28:19 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* Convert a list of char* nodes to argv (NULL-terminated). */
char	**list_to_argv(t_list *args)
{
	char	**argv;
	t_list	*tmp;
	int		count;
	int		i;

	count = 0;
	i = 0;
	tmp = args;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	argv = malloc(sizeof(char *) * (count + 1));
	if (!argv)
		return (NULL);
	tmp = args;
	while (tmp)
	{
		argv[i++] = tmp->content;
		tmp = tmp->next;
	}
	argv[i] = NULL;
	return (argv);
}

/* Finish building current command, link into list, and clear args nodes. */
void	finalize_cmd(t_command **head, t_command **current, t_list **args)
{
	t_list	*tmp;
	t_list	*next;

	if (!*current)
		return ;
	(*current)->argv = list_to_argv(*args);
	tmp = *args;
	while (tmp)
	{
		next = tmp->next;
		free(tmp);
		tmp = next;
	}
	*args = NULL;
	if (!*head)
		*head = *current;
	else
		last_command(*head)->next = *current;
	*current = NULL;
}
