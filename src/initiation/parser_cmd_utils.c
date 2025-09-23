/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cmd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:10:35 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/23 22:27:56 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
