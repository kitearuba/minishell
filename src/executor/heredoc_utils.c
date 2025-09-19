
#include "../../include/minishell.h"

char	*hd_expand_key(const char *s, size_t *i, t_bash *bash)
{
    size_t	start;
    char	*key;
    char	*val;

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
    val = ft_strdup(get_env_value(key, bash->env));
    free(key);
    return (val);
}

char	*hd_expand_line(const char *s, t_bash *bash)
{
    char	*res;
    char	*tmp;
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
            i++;
            tmp = hd_expand_key(s, &i, bash);
            res = append_and_free(res, tmp);
        }
        else
        {
            chunk = i;
            while (s[i] && s[i] != '$')
                i++;
            tmp = ft_substr(s, chunk, i - chunk);
            res = append_and_free(res, tmp);
        }
    }
    return (res);
}
