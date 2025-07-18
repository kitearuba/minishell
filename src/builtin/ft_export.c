
#include "../../include/minishell.h"

static int	append_env_var(char ***env, const char *new_var)
{
	int		i;
	char	**new_env;

	i = 0;
	while ((*env)[i])
		i++;
	new_env = malloc(sizeof(char *) * (i + 2));
	if (!new_env)
		return (1);
	i = -1;
	while ((*env)[++i])
		new_env[i] = ft_strdup((*env)[i]);
	new_env[i] = ft_strdup(new_var);
	new_env[i + 1] = NULL;
	free_2d_array(*env);
	*env = new_env;
	return (0);
}

static int	update_or_add_env(char ***env, const char *arg)
{
	int		i;
	char	*name;
	char	*equal_sign;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (0);
	name = ft_substr(arg, 0, equal_sign - arg);
	if (!name)
		return (1);
	i = 0;
	while ((*env)[i])
	{
		if (ft_strncmp((*env)[i], name, ft_strlen(name)) == 0
			&& (*env)[i][ft_strlen(name)] == '=')
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(arg);
			free(name);
			return (0);
		}
		i++;
	}
	free(name);
	return (append_env_var(env, arg));
}

static void	print_export_sorted(char **env)
{
	char	**copy;
	int		i;
	int		j;
	char	*tmp;

	copy = copy_env(env);
	if (!copy)
		return ;
	i = -1;
	while (copy[++i])
	{
		j = 0;
		while (copy[j + 1])
		{
			if (ft_strcmp(copy[j], copy[j + 1]) > 0)
			{
				tmp = copy[j];
				copy[j] = copy[j + 1];
				copy[j + 1] = tmp;
			}
			j++;
		}
	}
	i = -1;
	while (copy[++i])
		printf("declare -x %s\n", copy[i]);
	free_2d_array(copy);
}

int	ft_export(char **argv, t_bash *bash)
{
    int		i;
    int		status;
    char	*name;
    char	*eq;

    if (!argv[1])
    {
        print_export_sorted(bash->env);
        return (0);
    }
    i = 1;
    status = 0;
    while (argv[i])
    {
        eq = ft_strchr(argv[i], '=');
        if (eq)
            name = ft_substr(argv[i], 0, eq - argv[i]);
        else
            name = ft_strdup(argv[i]);
        if (!is_valid_identifier(name))
        {
            ft_printf_fd(STDERR_FILENO,
                "minishell: export: `%s': not a valid identifier\n", argv[i]);
            status = 1;
        }
        else if (update_or_add_env(&bash->env, argv[i]) != 0)
        {
            ft_putendl_fd("minishell: export: failed to update env", STDERR_FILENO);
            status = 1;
        }
        free(name);
        i++;
    }
    bash->exit_status = status;
    return (status);
}