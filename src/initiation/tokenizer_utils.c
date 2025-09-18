/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/18 21:00:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*new_token(t_token_type type, const char *start, size_t len,
	int quoted, int space_before)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_substr(start, 0, len);
	token->quoted = quoted;
	token->space_before = space_before;
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
	add_token(tokens, new_token(PIPE, &input[i], 1, 0, space_before));
	return (i + 1);
}

size_t	handle_word(const char *input, t_token **tokens, size_t i,
	int space_before)
{
	size_t	start;

	start = i;
	while (input[i]
		&& input[i] != ' ' && input[i] != '|' && input[i] != '<'
		&& input[i] != '>' && input[i] != '\'' && input[i] != '"'
		&& input[i] != '$')
		i++;
	add_token(tokens, new_token(WORD, &input[start], i - start, 0, space_before));
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