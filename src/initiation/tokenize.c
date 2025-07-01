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

static size_t  handle_pipe(const char *input, t_token **tokens, size_t i)
{
    add_token(tokens, new_token(PIPE, &input[i], 1));
    return (i + 1);
}

static size_t  handle_redirection(const char *input, t_token **tokens, size_t i)
{
    if (input[i] == '>' && input [i + 1] == '>')
    {
        add_token(tokens, new_token(REDIRECT_APPEND, &input[i], 2));
        return (i + 2);
    }
    if (input[i] == '<' && input[i + 1] == '<')
    {
        add_token(tokens, new_token(HEREDOC, &input[i], 2));
        return (input[i + 2]);
    }
    if (input[i] == '>')
    {
        add_token(tokens, new_token(REDIRECT_OUT, &input[i], 2));
        return (input[i + 1]);
    }
    add_token(tokens, new_token( REDIRECT_IN, &input[i], 1));
    return (input[i + 1]);
}

static int handle_quotes(const char *input, t_token **tokens, size_t *i)
{
    char            *quoted;
    t_token_type    type;

    quoted = extract_quoted_token(input, i);
    if (!quoted)
        return (0);
    if (input[*i - 1] == '\'')
        type = SINGLE_QUOTE;
    else
        type = DOUBLE_QUOTE;
    add_token(tokens, new_token(type,quoted, ft_strlen(quoted)));
    free(quoted);
    return (1);
}

static size_t handle_env_var(const char *input, t_token **tokens, size_t i)
{
    size_t  start;

    if (input[i + 1] == '?')
    {
        add_token(tokens, new_token(ENV_VAR, &input[i], 2));
        return (i + 2);
    }
    start = i + 1;
    i = start;
    while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
        i++;
    if (i == start)
    {
        add_token(tokens, new_token(WORD, &input[i - 1], 1));
        return (start);
    }
    add_token(tokens, new_token(ENV_VAR, &input[start - 1], i - start + 1));
    return (i);
}

t_token	*tokenize_input(const char *input)
{
    t_token	*tokens;
    size_t	i;

    tokens = NULL;
    i = 0;
    while (input[i])
    {
        if (input[i] == ' ')
            i++;
        else if (input[i] == '|')
            i = handle_pipe(input, &tokens, i);
        else if (input[i] == '>' || input[i] == '<')
            i = handle_redirection(input, &tokens, i);
        else if (input[i] == '\'' || input[i] == '"')
        {
            if (!handle_quotes(input, &tokens, &i))
            {
                printf("Syntax error: unmatched quote\n");
                free_tokens(tokens);
                return (NULL);
            }
        }
        else if (input[i] == '$')
            i = handle_env_var(input, &tokens, i);
        else
            i = handle_word(input, &tokens, i);
    }
    return (tokens);
}