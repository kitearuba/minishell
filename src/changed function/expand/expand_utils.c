/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/18 23:55:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_env_value(const char *key, char **env)
{
	int		i;
	size_t	key_len;

	i = 0;
	key_len = ft_strlen(key);
	while (env[i])
	{
		if (!ft_strncmp(env[i], key, key_len) && env[i][key_len] == '=')
			return (env[i] + key_len + 1);
		i++;
	}
	return ("");
}

char	*append_and_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

char	*expand_token_value(t_token *token, t_bash *bash)
{
	char	*name;
	char	*val;

	if (!token->value || token->value[0] != '$')
		return (ft_strdup(token->value));
	if (!ft_strncmp(token->value, "$?", 2))
		return (ft_itoa(bash->exit_status));
	name = ft_substr(token->value, 1, ft_strlen(token->value) - 1);
	if (!name)
		return (NULL);
	val = ft_strdup(get_env_value(name, bash->env));
	free(name);
	return (val);
}
