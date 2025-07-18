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

t_token	*new_token(t_token_type type, const char *start, size_t len)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->type = type;
	token->value = ft_substr(start, 0, len);
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

size_t	handle_pipe(const char *input, t_token **tokens, size_t i)
{
	add_token(tokens, new_token(PIPE, &input[i], 1));
	return (i + 1);
}

size_t	handle_word(const char *input, t_token **tokens, size_t i)
{
	size_t	start;

	start = i;
	while (input[i]
		&& input[i] != ' '
		&& input[i] != '|'
		&& input[i] != '<'
		&& input[i] != '>'
		&& input[i] != '\''
		&& input[i] != '"'
		&& input[i] != '$')
		i++;
	add_token(tokens, new_token(WORD, &input[start], i - start));
	return (i);
}

char	*extract_quoted_token(const char *line, size_t *index)
{
	char	quote_char;
	size_t	start;
	size_t	end;
	char	*token;

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
