/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 14:10:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/18 23:48:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*expand_token_value(t_token *token, t_bash *bash)
{
	char	*variable_name;
	char	*expanded;

	if (ft_strncmp(token->value, "$?", 2) == 0)
		return (ft_itoa(bash->exit_status));
	variable_name = ft_substr(token->value, 1, ft_strlen(token->value) - 1);
	if (!variable_name)
		return (NULL);
	expanded = ft_strdup(get_env_value(variable_name, bash->env));
	free(variable_name);
	return (expanded);
}

static char	*extract_var_value(char **str, t_bash *bash)
{
	int		len;
	char	*var;
	char	*val;

	if (**str == '?')
	{
		(*str)++;
		return (ft_itoa(bash->exit_status));
	}
	len = 0;
	while ((*str)[len] && (ft_isalnum((*str)[len]) || (*str)[len] == '_'))
		len++;
	var = ft_substr(*str, 0, len);
	val = ft_strdup(get_env_value(var, bash->env));
	free(var);
	*str += len;
	return (val);
}

static char	*expand_inside_double_quote(const char *str, t_bash *bash)
{
	char	*result;
	char	*tmp;
	char	*val;
	char	*scan;
	int		i;

	scan = (char *)str;
	result = ft_strdup("");
	while (*scan)
	{
		i = 0;
		while (scan[i] && scan[i] != '$')
			i++;
		tmp = ft_substr(scan, 0, i);
		result = append_and_free(result, tmp);
		scan += i;
		if (*scan == '$')
		{
			scan++;
			val = extract_var_value(&scan, bash);
			result = append_and_free(result, val);
		}
	}
	return (result);
}

static void	expand_token_if_needed(t_token *tok, t_bash *bash)
{
	char	*expanded;

	if (tok->type == ENV_VAR)
	{
		expanded = expand_token_value(tok, bash);
		if (expanded)
		{
			free(tok->value);
			tok->value = expanded;
			tok->type = WORD;
		}
	}
	else if (tok->type == DOUBLE_QUOTE)
	{
		expanded = expand_inside_double_quote(tok->value, bash);
		if (expanded)
		{
			free(tok->value);
			tok->value = expanded;
			tok->type = WORD;
		}
	}
}

void	expand_env_vars(t_token *tokens, t_bash *bash)
{
	t_token	*current;

	current = tokens;
	while (current)
	{
		expand_token_if_needed(current, bash);
		current = current->next;
	}
}
