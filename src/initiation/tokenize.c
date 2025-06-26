/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/05/20 16:43:48 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void print_tokens(t_token *list)
{
    while (list)
    {
        printf("[%d: \"%s\"] -> ", list->type, list->value);
        list = list->next;
    }
    printf("NULL\n");
}

static t_token *new_token(t_token_type type, const char *start, size_t len)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = ft_substr(start, 0, len);
    token->next = NULL;
    return (token);
}

static void add_token(t_token **head, t_token *new)
{
    t_token *temp;

    temp = *head;
    if (!temp)
    {
        *head =new;
        return ;
    }
    while (temp->next)
        temp = temp->next;
    temp->next = new;
}

t_token *tokenize_input(const char *input)
{
    t_token *tokens;
    size_t  i;
    size_t  start;

    tokens = NULL;
    i = 0;
    while (input[i])
    {
        if (input[i] == ' ')
            i++;
        else if  (input[i] == '|')
        {
            add_token(&tokens, new_token(PIPE, &input[i], 1));
            i++;
        }
        else
        {
            start = i;
            while (input[i] && input[i] != ' ' && input[i] != '|')
                i++;
            add_token(&tokens, new_token(WORD, &input[start], i - start));
        }
    }
    return (tokens);
}