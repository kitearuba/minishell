#include "../../include/minishell.h"

/* ************************************************************************** */
/*                                                                            */
/*                               new_token                                   */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Creates a new token with the specified type, extracts its value         */
/*    from the input string, and initializes next to NULL.                    */
/*                                                                            */
/*  Parameters:                                                               */
/*  - type: the type of the token to create.                                  */
/*  - start: pointer to the start of the token substring in the input.        */
/*  - len: length of the token substring.                                     */
/*                                                                            */
/*  Return:                                                                   */
/*  - Pointer to the newly allocated t_token, or NULL on allocation failure.  */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Uses malloc and ft_substr (from libft).                                 */
/*                                                                            */
/* ************************************************************************** */
t_token *new_token(t_token_type type, const char *start, size_t len)
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

/* ************************************************************************** */
/*                                                                            */
/*                               add_token                                   */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Adds a new token to the end of the token linked list.                   */
/*                                                                            */
/*  Parameters:                                                               */
/*  - head: pointer to the pointer of the first token in the list.            */
/*  - new: the new token to add.                                              */
/*                                                                            */
/*  Return:                                                                   */
/*  - None.                                                                   */
/*                                                                            */
/* ************************************************************************** */
void add_token(t_token **head, t_token *new)
{
    t_token *temp;

    if (!*head)
    {
        *head =new;
        return ;
    }
    temp = *head;
    while (temp->next)
        temp = temp->next;
    temp->next = new;
}

/* ************************************************************************** */
/*                                                                            */
/*                              handle_word                                  */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Handles a word token (a sequence of non-special characters),            */
/*    extracts it, and adds it to the token list.                             */
/*                                                                            */
/*  Parameters:                                                               */
/*  - input: the user input string.                                           */
/*  - tokens: pointer to the token list.                                      */
/*  - i: current index in the input string.                                   */
/*                                                                            */
/*  Return:                                                                   */
/*  - The updated index after processing the word.                            */
/*                                                                            */
/* ************************************************************************** */
size_t  handle_word(const char *input, t_token **tokens, size_t i)
{
    size_t  start;

    start = i;
    while (input[i] && input[i] != ' ' && input[i] != '|' &&
        input[i] != '<' && input[i] != '>' && input[i] != '\'' &&
        input[i] != '"' && input[i] != '$')
        i++;
    add_token(tokens, new_token(WORD, &input[start], i - start));
    return (i);
}

/* ************************************************************************** */
/*                                                                            */
/*                          extract_quoted_token                             */
/* ************************************************************************** */
/*                                                                            */
/*  Description:                                                              */
/*  - Extracts a substring inside matching single or double quotes starting   */
/*    at the current index. Returns the extracted string without quotes.      */
/*                                                                            */
/*  Parameters:                                                               */
/*  - line: the input string.                                                 */
/*  - index: pointer to the current index in the string (updated in place).   */
/*                                                                            */
/*  Return:                                                                   */
/*  - Newly allocated string containing the quoted content, or NULL if        */
/*    unmatched quote is found.                                               */
/*                                                                            */
/*  Notes:                                                                    */
/*  - Caller is responsible for freeing the returned string.                  */
/*                                                                            */
/* ************************************************************************** */
char    *extract_quoted_token(const char *line, size_t *index)
{
    char    quote_char;
    size_t  start;
    size_t  end;
    char    *token;

    quote_char = line[*index];
    (*index)++;
    start = *index;
    while (line[*index] && line[*index] != quote_char)
        (*index)++;
    if (line[*index] != quote_char)
        return (NULL);
    end = *index;
    (*index)++;
    token = ft_substr(&line[start], 0, end - start);
    return (token);
}
