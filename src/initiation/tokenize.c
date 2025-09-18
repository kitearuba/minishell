/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/18 20:45:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static size_t	handle_redirect(const char *in, t_token **toks, size_t i,
	int space)
{
	if (in[i] == '>' && in[i + 1] == '>')
	{
		add_token(toks, new_token(REDIRECT_APPEND, &in[i], 2, 0, space));
		i += 2;
	}
	else if (in[i] == '<' && in[i + 1] == '<')
	{
		add_token(toks, new_token(HEREDOC, &in[i], 2, 0, space));
		i += 2;
	}
	else if (in[i] == '>')
	{
		add_token(toks, new_token(REDIRECT_OUT, &in[i], 1, 0, space));
		i++;
	}
	else
	{
		add_token(toks, new_token(REDIRECT_IN, &in[i], 1, 0, space));
		i++;
	}
	while (in[i] == ' ')
		i++;
	return (i);
}

int	handle_quotes(const char *in, t_token **toks, size_t *i, int space)
{
	char	*quoted;
	int		quoted_flag;

	if (in[*i] == '\'')
		quoted_flag = 1;
	else
		quoted_flag = 2;
	quoted = extract_quoted_token(in, i);
	if (!quoted)
		return (0);
	add_token(toks, new_token(WORD, quoted, ft_strlen(quoted),
			quoted_flag, space));
	free(quoted);
	return (1);
}

size_t	handle_env_var(const char *in, t_token **toks, size_t i, int space)
{
	size_t	start;

	if (in[i + 1] == '?')
	{
		add_token(toks, new_token(ENV_VAR, &in[i], 2, 0, space));
		return (i + 2);
	}
	start = i + 1;
	i = start;
	while (in[i] && (ft_isalnum(in[i]) || in[i] == '_'))
		i++;
	if (i == start)
	{
		add_token(toks, new_token(WORD, &in[i - 1], 1, 0, space));
		return (start);
	}
	add_token(toks, new_token(ENV_VAR, &in[start - 1], i - start + 1, 0, space));
	return (i);
}

static int	dispatch_token(const char *in, t_token **toks, size_t *i, int space)
{
	if (in[*i] == '|')
		*i = handle_pipe(in, toks, *i, space);
	else if (in[*i] == '>' || in[*i] == '<')
		*i = handle_redirect(in, toks, *i, space);
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