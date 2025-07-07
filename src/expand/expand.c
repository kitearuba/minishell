/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:10:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/07 21:15:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char *get_

static char *expand_token_value(t_token *current, t_bash *bash)
{
    char    *variable_name;
    char    *expanded;

    if (ft_strncmp (token->value, "$?", 2) == 0)
        return (ft_itoa(bash->exit));
    variable_name = ft_substr(token->value,1, ft_stlen(token->value) - 1);
    if (!variable_name)
        return (NULL);
    expanded = ft_strdup(get_env_value(variable_name, bash->env));
    free (variable_name);
    return (expanded);
}

void    expand_env_vars(t_token *tokens, t_bash *bash)
{
    t_token *current;
    char    *expanded;

    current = tokens;
    while (current)
    {
        if (current->type == ENV_VAR)
        {
            expanded = expand_token_value (current, bash);
            if (expanded)
            {
                free (current->value);
                current->value = expanded;
                current->type = WORD;
            }
        }
        current = current->next;
    }
}