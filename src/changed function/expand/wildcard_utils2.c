/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard_utils2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsamy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 16:00:02 by bsamy             #+#    #+#             */
/*   Updated: 2025/09/19 16:00:45 by bsamy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*get_tail(t_token *head)
{
	while (head && head->next)
		head = head->next;
	return (head);
}

struct dirent	*get_next_entry(DIR *dir)
{
	return (readdir(dir));
}
