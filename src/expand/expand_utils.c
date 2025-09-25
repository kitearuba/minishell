/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:31:34 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Expansion helpers shared by the expander                                   */
/* -------------------------------------------------------------------------- */

/*
** get_env_value
** -------------
** Return a pointer into env (or an empty string "") for key. Does not alloc.
** Note: caller must duplicate if it needs ownership.
*/
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

/*
** append_and_free
** ---------------
** Concatenate s1 + s2 into a newly-allocated string and free both inputs.
** Returns the joined pointer (or NULL on allocation failure).
*/
char	*append_and_free(char *s1, char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (joined);
}

/*
** expand_token_value
** ------------------
** Expand a token that *starts* with '$' (ENV_VAR token):
**   - "$?" -> itoa(exit_status)
**   - "$NAME" -> strdup(value or "")
**   - otherwise -> strdup(original token->value)
** Returns a malloc'ed string.
*/
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
