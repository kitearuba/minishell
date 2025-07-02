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

/* ************************************************************************** */
/*                                                                            */
/*                               print_tokens                                 */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Prints the token list to stdout in a formatted way for debugging,       */
/*    showing each token's type and value.                                    */
/*                                                                            */
/*  Parameters:                                                               */
/*  - list: pointer to the first token in the linked list.                    */
/*                                                                            */
/* ************************************************************************** */
void print_tokens(t_token *list)
{
    while (list)
    {
        printf("[%d: \"%s\"] -> ", list->type, list->value);
        list = list->next;
    }
    printf("NULL\n");
}

/* ************************************************************************** */
/*                                                                            */
/*                               handle_pipe                                  */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Handles the '|' character by creating a PIPE token and adding it        */
/*    to the token list.                                                      */
/*                                                                            */
/*  Parameters:                                                               */
/*  - input: the user input string.                                           */
/*  - tokens: pointer to the token list.                                      */
/*  - i: current index in the input string.                                   */
/*                                                                            */
/*  Return:                                                                   */
/*  - The updated index after processing the pipe.                            */
/*                                                                            */
/* ************************************************************************** */
static size_t  handle_pipe(const char *input, t_token **tokens, size_t i)
{
    add_token(tokens, new_token(PIPE, &input[i], 1));
    return (i + 1);
}

/* ************************************************************************** */
/*                                                                            */
/*                            handle_redirection                              */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Detects and handles redirection operators (>, >>, <, <<), creates       */
/*    corresponding tokens, and skips trailing spaces.                        */
/*                                                                            */
/*  Parameters:                                                               */
/*  - input: the user input string.                                           */
/*  - tokens: pointer to the token list.                                      */
/*  - i: current index in the input string.                                   */
/*                                                                            */
/*  Return:                                                                   */
/*  - The updated index after processing the redirection.                     */
/*                                                                            */
/* ************************************************************************** */
static size_t  handle_redirection(const char *input, t_token **tokens, size_t i)
{
    if (input[i] == '>' && input [i + 1] == '>')
    {
        add_token(tokens, new_token(REDIRECT_APPEND, &input[i], 2));
        i += 2;
    }
    else if (input[i] == '<' && input[i + 1] == '<')
    {
        add_token(tokens, new_token(HEREDOC, &input[i], 2));
        i += 2;
    }
    else if (input[i] == '>')
    {
        add_token(tokens, new_token(REDIRECT_OUT, &input[i], 1));
        i++;
    }
    else
    {
        add_token(tokens, new_token( REDIRECT_IN, &input[i], 1));
        i++;
    }
    while (input[i] == ' ')
        i++;
    return (i);
}

/* ************************************************************************** */
/*                                                                            */
/*                               handle_quotes                                */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Extracts a quoted string starting at the current index, determines      */
/*    if it is a single or double quote, and adds the corresponding token.    */
/*                                                                            */
/*  Parameters:                                                               */
/*  - input: the user input string.                                           */
/*  - tokens: pointer to the token list.                                      */
/*  - i: pointer to the current index in the input string (updated in place). */
/*                                                                            */
/*  Return:                                                                   */
/*  - 1 if a quoted token is successfully processed.                          */
/*  - 0 if unmatched quote is detected.                                       */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Frees temporary extracted string after use.                             */
/*                                                                            */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                                                            */
/*                             handle_env_var                                 */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Handles environment variable tokens starting with '$', including        */
/*    special case for '$?'. Adds ENV_VAR tokens or falls back to WORD token  */
/*    if variable name is invalid.                                            */
/*                                                                            */
/*  Parameters:                                                               */
/*  - input: the user input string.                                           */
/*  - tokens: pointer to the token list.                                      */
/*  - i: current index in the input string.                                   */
/*                                                                            */
/*  Return:                                                                   */
/*  - The updated index after processing the environment variable.            */
/*                                                                            */
/* ************************************************************************** */
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

/* ************************************************************************** */
/*                                                                            */
/*                             tokenize_input                                 */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Main tokenizer loop. Iterates over input string, dispatches             */
/*    characters to appropriate handlers to build the token list, and         */
/*    returns the list of tokens.                                             */
/*                                                                            */
/*  Parameters:                                                               */
/*  - input: the user input string.                                           */
/*                                                                            */
/*  Return:                                                                   */
/*  - Pointer to the head of the token linked list.                           */
/*  - Returns NULL if a syntax error occurs (e.g., unmatched quote).          */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Skips spaces.                                                           */
/*  - Calls handle_pipe, handle_redirection, handle_quotes, handle_env_var,   */
/*    or handle_word (not shown here) based on character.                     */
/*  - Prints syntax errors (e.g., unmatched quotes) to stderr using           */
/*    ft_printf_fd() before freeing tokens and returning NULL.                */
/*                                                                            */
/* ************************************************************************** */
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
                ft_printf_fd(2, "minishell: syntax error: unmatched quote\n");
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