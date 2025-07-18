#include "../../include/minishell.h"

int is_valid_identifier(const char *str)
{
    int i;

    if (!str || !ft_isalpha(str[0]))
        return (0);
    i = 1;
    while (str[i])
    {
        if (!ft_isalnum(str[i]) && str[i] != '_')
            return (0);
        i++;
    }
    return (1);
}

char	**copy_env(char **env)
{
	int		count;
	char	**copy;
	int		i;

	count = 0;
	while (env[count])
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (!copy)
		return (NULL);
	i = 0;
	while (i < count)
	{
		copy[i] = ft_strdup(env[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}
