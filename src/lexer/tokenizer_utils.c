/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 18:48:11 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
