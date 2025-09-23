/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:10:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/23 23:46:56 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
	var = ft_substr(*s, 0, len);
	val = ft_strdup(get_env_value(var, bash->env));
	free(var);
	*s += len;
	return (val);
}

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
	(*cur)->type = WORD;
	*prev = *cur;
	*cur = (*cur)->next;
}

void	expand_one(t_token **head, t_token **prev, t_token **cur, t_bash *b)
{
	char	*exp;

	if ((*cur)->type == ENV_VAR)
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
			(*cur)->type = WORD;
		}
	}
	*prev = *cur;
	*cur = (*cur)->next;
}

void	expand_env_vars(t_token **tokens, t_bash *bash)
{
	t_token	*prev;
	t_token	*cur;

	prev = NULL;
	cur = *tokens;
	while (cur)
		expand_one(tokens, &prev, &cur, bash);
}
