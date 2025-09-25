/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:22:32 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Small helpers used by the tokenizer.                                       */
/* -------------------------------------------------------------------------- */

/*
** new_token
** ---------
** Allocate and initialize a token with given type, substring [start, len),
** quoted flag, and default metadata (space_before=0, next=NULL).
** Returns the allocated token or NULL on allocation failure.
*/
t_token	*new_token(t_token_type type, const char *start, size_t len, int quoted)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_substr(start, 0, len);
	token->quoted = quoted;
	token->space_before = 0;
	token->next = NULL;
	return (token);
}

/*
** add_token
** ---------
** Append 'new' at the tail of the token list pointed by 'head'.
** If the list is empty, 'new' becomes the head.
*/
void	add_token(t_token **head, t_token *new)
{
	t_token	*temp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	temp = *head;
	while (temp->next)
		temp = temp->next;
	temp->next = new;
}

/*
** handle_pipe
** -----------
** Create a pipe token ('|') at index i with the given space_before flag,
** append it, and return the next index (i + 1).
*/
size_t	handle_pipe(const char *input, t_token **tokens, size_t i,
		int space_before)
{
	t_token	*tok;

	tok = new_token(pipe_tok, &input[i], 1, 0);
	if (!tok)
		return (i + 1);
	tok->space_before = space_before;
	add_token(tokens, tok);
	return (i + 1);
}

/*
** handle_word
** -----------
** Consume a WORD starting at index i until a metachar is found:
** space, |, <, >, quote, or $. Create a WORD token, append it, and
** return the new index where tokenizing should continue.
*/
size_t	handle_word(const char *input, t_token **tokens, size_t i,
		int space_before)
{
	size_t	start;
	t_token	*tok;

	start = i;
	while (input[i]
		&& input[i] != ' ' && input[i] != '|' && input[i] != '<'
		&& input[i] != '>' && input[i] != '\'' && input[i] != '"'
		&& input[i] != '$')
		i++;
	tok = new_token(word, &input[start], i - start, 0);
	if (!tok)
		return (i);
	tok->space_before = space_before;
	add_token(tokens, tok);
	return (i);
}

/*
** extract_quoted_token
** --------------------
** Given line[*index] == '\'' or '"', return a freshly-allocated substring
** containing the contents inside the matching closing quote. On success,
** *index is advanced to the first char after the closing quote.
** Returns NULL if the closing quote is missing.
*/
char	*extract_quoted_token(const char *line, size_t *index)
{
	char	quote_char;
	size_t	start;
	size_t	end;

	quote_char = line[*index];
	(*index)++;
	start = *index;
	while (line[*index] && line[*index] != quote_char)
		(*index)++;
	if (line[*index] != quote_char)
		return (NULL);
	end = *index;
	(*index)++;
	return (ft_substr(&line[start], 0, end - start));
}
