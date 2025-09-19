/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/17 10:13:31 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 14:14:44 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*hd_expand_key(const char *s, size_t *i, t_bash *bash)
{
	size_t	start;
	char	*key;
	char	*val;
	char	*raw;

	if (s[*i] == '?')
	{
		(*i)++;
		return (ft_itoa(bash->exit_status));
	}
	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	if (*i == start)
		return (ft_strdup("$"));
	key = ft_substr(s, start, *i - start);
	raw = get_env_value(key, bash->env);
	if (raw)
		val = ft_strdup(raw);
	else
		val = ft_strdup("");
	free(key);
	return (val);
}

static int	append_chunk(char **res, const char *s, size_t start, size_t end)
{
	char	*tmp;

	tmp = ft_substr(s, start, end - start);
	*res = append_and_free(*res, tmp);
	if (!*res)
		return (1);
	return (0);
}

static int	append_expansion(char **res, const char *s, size_t *i, t_bash *bash)
{
	char	*tmp;

	(*i)++;
	tmp = hd_expand_key(s, i, bash);
	*res = append_and_free(*res, tmp);
	if (!*res)
		return (1);
	return (0);
}

char	*hd_expand_line(const char *s, t_bash *bash)
{
	char	*res;
	size_t	i;
	size_t	chunk;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	i = 0;
	while (s[i])
	{
		if (s[i] == '$')
		{
			if (append_expansion(&res, s, &i, bash))
				return (NULL);
		}
		else
		{
			chunk = i;
			while (s[i] && s[i] != '$')
				i++;
			if (append_chunk(&res, s, chunk, i))
				return (NULL);
		}
	}
	return (res);
}
