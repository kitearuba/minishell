/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:10:35 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:24:14 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* -------------------------------------------------------------------------- */
/* Parser: list → argv conversion and command finalization                    */
/* -------------------------------------------------------------------------- */

/*
** list_len
** --------
** Count number of nodes in a t_list* of argument strings.
** Returns the length (>= 0).
*/
static int	list_len(t_list *args)
{
	int		n;
	t_list	*tmp;

	n = 0;
	tmp = args;
	while (tmp)
	{
		n++;
		tmp = tmp->next;
	}
	return (n);
}

/*
** list_to_argv
** ------------
** Convert a t_list* of char* into a NULL-terminated argv vector.
** Duplicates each string. Returns malloc'ed **argv or NULL on failure.
*/
char	**list_to_argv(t_list *args)
{
	char	**argv;
	int		n;
	int		i;

	n = list_len(args);
	argv = malloc(sizeof(char *) * (n + 1));
	if (!argv)
		return (NULL);
	i = 0;
	while (args)
	{
		argv[i] = ft_strdup((char *)args->content);
		if (!argv[i])
		{
			while (i > 0)
				free(argv[--i]);
			free(argv);
			return (NULL);
		}
		i++;
		args = args->next;
	}
	argv[i] = NULL;
	return (argv);
}

/*
** finalize_cmd
** ------------
** Attach current args list to *current as argv, free the list nodes,
** and append *current to the command chain (*head). Sets *current = NULL.
*/
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
		free(tmp->content);
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
