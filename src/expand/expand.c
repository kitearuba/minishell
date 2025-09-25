/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:10:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:18:54 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* -------------------------------------------------------------------------- */
/* Variable expansion & quote-aware rewrites                                  */
/* -------------------------------------------------------------------------- */

/*
** extract_var_value
** -----------------
** Given *s positioned at the first char after a '$', extract the variable
** reference and return a freshly allocated string with its value:
**   - '?'  -> bash->exit_status as string, advances *s by 1
**   - NAME -> value of NAME (alnum or '_'), advances *s by |NAME|
**   - ''   -> literal "$" when no valid name follows
** Returns: malloc'ed value (caller owns).
*/
static char	*extract_var_value(char **s, t_bash *bash)
{
	int		len;
	char	*var;
	char	*val;

	if (**s == '?')
	{
		(*s)++;
		return (ft_itoa(bash->exit_status));
	}
	len = 0;
	while ((*s)[len] && (ft_isalnum((*s)[len]) || (*s)[len] == '_'))
		len++;
	if (len == 0)
		return (ft_strdup("$"));
	var = ft_substr(*s, 0, len);
	val = ft_strdup(get_env_value(var, bash->env));
	free(var);
	*s += len;
	return (val);
}

/*
** expand_in_double_quotes
** -----------------------
** Expand a str that was originally inside double quotes. Performs $-expansion
** but preserves all other characters verbatim. Returns a malloc'ed result.
*/
static char	*expand_in_double_quotes(const char *str, t_bash *bash)
{
	char	*res;
	char	*tmp;
	char	*val;
	char	*scan;
	int		n;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	scan = (char *)str;
	while (*scan)
	{
		n = 0;
		while (scan[n] && scan[n] != '$')
			n++;
		tmp = ft_substr(scan, 0, n);
		res = append_and_free(res, tmp);
		scan += n;
		if (*scan++ == '$')
		{
			val = extract_var_value(&scan, bash);
			res = append_and_free(res, val);
		}
	}
	return (res);
}

/*
** expand_env_var
** --------------
** Expand a single ENV_VAR token:
**   - If unquoted and expansion yields empty string, remove the token.
**   - Otherwise replace token->value with expanded content and set type=word.
** On removal, relinks the list (using head/prev/cur).
*/
void	expand_env_var(t_token **head, t_token **prev, t_token **cur, t_bash *b)
{
	t_token	*next;
	char	*exp;

	exp = expand_token_value(*cur, b);
	if (!exp)
		exp = ft_strdup("");
	if ((*cur)->quoted == 0 && exp[0] == '\0')
	{
		next = (*cur)->next;
		if (*prev)
			(*prev)->next = next;
		else
			*head = next;
		free((*cur)->value);
		free(*cur);
		*cur = next;
		free (exp);
		return ;
	}
	free((*cur)->value);
	(*cur)->value = exp;
	(*cur)->type = word;
	*prev = *cur;
	*cur = (*cur)->next;
}

/*
** expand_one
** ----------
** Expand one token according to its type/quote:
**   - ENV_VAR -> expand_env_var
**   - quoted==2 (double quotes) -> expand inside the string
** Advances *cur (and updates *prev).
*/
void	expand_one(t_token **head, t_token **prev, t_token **cur, t_bash *b)
{
	char	*exp;

	if ((*cur)->type == env_var)
	{
		expand_env_var(head, prev, cur, b);
		return ;
	}
	if ((*cur)->quoted == 2)
	{
		exp = expand_in_double_quotes((*cur)->value, b);
		if (exp)
		{
			free((*cur)->value);
			(*cur)->value = exp;
			(*cur)->type = word;
		}
	}
	*prev = *cur;
	*cur = (*cur)->next;
}

/*
** expand_env_vars
** ---------------
** Walk the token list and apply the expansion rules to every token.
*/
void	expand_env_vars(t_token **tokens, t_bash *bash)
{
	t_token	*prev;
	t_token	*cur;

	prev = NULL;
	cur = *tokens;
	while (cur)
		expand_one(tokens, &prev, &cur, bash);
}
