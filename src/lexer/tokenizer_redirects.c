/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_redirects.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/19 18:48:28 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 19:04:45 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_token	*make_redirect_tok(const char *in, size_t i, size_t *adv)
{
	t_token	*tok;

	tok = NULL;
	if (in[i] == '>' && in[i + 1] == '>')
	{
		*adv = 2;
		tok = new_token(redirect_append, &in[i], 2, 0);
	}
	else if (in[i] == '<' && in[i + 1] == '<')
	{
		*adv = 2;
		tok = new_token(heredoc_tok, &in[i], 2, 0);
	}
	else
	{
		*adv = 1;
		if (in[i] == '>')
			tok = new_token(redirect_out, &in[i], 1, 0);
		else
			tok = new_token(redirect_in, &in[i], 1, 0);
	}
	return (tok);
}

size_t	append_redirect_token(const char *in, t_token **toks, size_t i,
		int space)
{
	t_token	*tok;
	size_t	adv;

	tok = make_redirect_tok(in, i, &adv);
	i += adv;
	if (tok)
	{
		tok->space_before = space;
		add_token(toks, tok);
	}
	while (in[i] == ' ')
		i++;
	return (i);
}
