/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:29:54 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Tokenize: split a raw input line into a linked list of t_token nodes.      */
/* -------------------------------------------------------------------------- */

/*
** push_tok
** --------
** Set the token's space_before flag and append it to the list (NULL-safe).
** Parameters:
**   toks  - pointer to the head of the token list
**   tok   - token to append; if NULL, does nothing
**   space - 1 if a space preceded this token, 0 otherwise
*/
static void	push_tok(t_token **toks, t_token *tok, int space)
{
	if (!tok)
		return ;
	tok->space_before = space;
	add_token(toks, tok);
}

/*
** handle_quotes
** -------------
** Extract a single- or double-quoted chunk starting at in[*i], create a WORD
** token (quoted flag = 1 for single quotes, 2 for double quotes), push it, and
** advance *i to the first char after the closing quote.
** Returns 1 on success, 0 on unmatched quote (and prints an error elsewhere).
*/
int	handle_quotes(const char *in, t_token **toks, size_t *i, int space)
{
	char	*quoted;
	int		quoted_flag;
	t_token	*tok;

	if (in[*i] == '\'')
		quoted_flag = 1;
	else
		quoted_flag = 2;
	quoted = extract_quoted_token(in, i);
	if (!quoted)
		return (0);
	tok = new_token(word, quoted, ft_strlen(quoted), quoted_flag);
	push_tok(toks, tok, space);
	free(quoted);
	return (1);
}

/*
** handle_env_var
** --------------
** Parse an env-var token starting at the '$' at index i.
**   - "$?"     => ENV_VAR of length 2
**   - "$NAME"  => ENV_VAR spanning NAME (alnum or '_')
**   - "$" not followed by NAME => literal "$" as a WORD
** Pushes the token and returns the new index after the parsed segment.
*/
size_t	handle_env_var(const char *in, t_token **toks, size_t i, int space)
{
	size_t	start;
	t_token	*tok;

	start = 0;
	tok = NULL;
	if (in[i + 1] == '?')
	{
		tok = new_token(env_var, &in[i], 2, 0);
		push_tok(toks, tok, space);
		return (i + 2);
	}
	start = i + 1;
	i = start;
	while (in[i] && (ft_isalnum(in[i]) || in[i] == '_'))
		i++;
	if (i == start)
	{
		tok = new_token(word, &in[i - 1], 1, 0);
		push_tok(toks, tok, space);
		return (start);
	}
	tok = new_token(env_var, &in[start - 1], i - start + 1, 0);
	push_tok(toks, tok, space);
	return (i);
}

/*
** dispatch_token
** --------------
** Route the current char to the right tokenizer:
**   '|' -> handle_pipe
**   '<' or '>' -> append_redirect_token
**   '\'' or '"' -> handle_quotes
**   '$' -> handle_env_var
**   default -> handle_word
** Returns 1 on success, 0 on fatal error (e.g., unmatched quote).
*/
static int	dispatch_token(const char *in, t_token **toks, size_t *i, int space)
{
	if (in[*i] == '|')
		*i = handle_pipe(in, toks, *i, space);
	else if (in[*i] == '>' || in[*i] == '<')
		*i = append_redirect_token(in, toks, *i, space);
	else if (in[*i] == '\'' || in[*i] == '"')
	{
		if (!handle_quotes(in, toks, i, space))
		{
			ft_printf_fd(2, "minishell: syntax error: unmatched quote\n");
			free_tokens(*toks);
			return (0);
		}
	}
	else if (in[*i] == '$')
		*i = handle_env_var(in, toks, *i, space);
	else
		*i = handle_word(in, toks, *i, space);
	return (1);
}

/*
** tokenize_input
** --------------
** Main entry point for lexing. Skips spaces, sets space_before per token, and
** builds the full token list for the input string. Returns the head of the
** list (or NULL on fatal error such as unmatched quotes).
*/
t_token	*tokenize_input(const char *input)
{
	t_token	*tokens;
	size_t	i;
	int		space;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		space = 0;
		while (input[i] == ' ')
		{
			space = 1;
			i++;
		}
		if (!input[i])
			break ;
		if (!dispatch_token(input, &tokens, &i, space))
			return (NULL);
	}
	return (tokens);
}
