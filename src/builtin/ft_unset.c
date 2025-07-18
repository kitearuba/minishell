#include "../../include/minishell.h"

static int    ft_arrlen(char **arr)
{
    int    len;

    len = 0;
    while (arr && arr[len])
        len++;
    return (len);
}

static void    copy_env_excluding(char **new_env, char **old_env, char *key)
{
    int    i;
    int    j;
    size_t    key_len;

    i = 0;
    j = 0;
    key_len = ft_strlen(key);
    while (old_env[i])
    {
        if (ft_strncmp(old_env[i], key, key_len) == 0 && old_env[i][key_len] == '=')
            i++;
        else
        {
            new_env[j] = ft_strdup(old_env[i]);
            j++;
            i++;
        }
    }
    new_env[j] = NULL;
}

static void    remove_env_var(t_bash *bash, char *key)
{
    int    new_len;
    char    **new_env;

    new_len = ft_arrlen(bash->env);
    new_env = malloc(sizeof(char *) * new_len);
    if (!new_env)
        return ;
    copy_env_excluding(new_env, bash->env, key);
    free_2d_array(bash->env);
    bash->env = new_env;
}

int ft_unset(char **argv, t_bash *bash)
{
    int    i;
    int    status;

    if (!argv || !bash)
        return (1);
    status = 0;
    i = 1;
    while (argv[i])
    {
        if (!is_valid_identifier(argv[i]))
        {
            ft_printf_fd(STDERR_FILENO,
                "minishell: unset: `%s': not a valid identifier\n", argv[i]);
            status = 1;
        }
        else
            remove_env_var(bash, argv[i]);
        i++;
    }
    bash->exit_status = status;
    return (0);
}