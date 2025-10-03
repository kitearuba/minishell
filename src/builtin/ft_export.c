/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:43:48 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:33:59 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Update existing KEY=... or append new var                                  */
/* -------------------------------------------------------------------------- */
/*
** update_or_add_env (file-local)
** ------------------------------
** If `arg` contains '=', replaces KEY's entry; otherwise no-op (returns 0).
**
** Params:  env : char*** environment vector ref
**          arg : "KEY" or "KEY=VAL"
** Return:  int  0 on success, 1 on alloc failure
*/
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

/* -------------------------------------------------------------------------- */
/* In-place bubble sort of a NULL-terminated vector                           */
/* -------------------------------------------------------------------------- */
static void	sort_env_copy(char **copy)
{
	int		i;
	int		j;
	char	*tmp;

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
}

/* print one line in bash style: declare -x KEY="VAL" or declare -x KEY */
static void print_export_line(const char *entry)
{
    const char *eq;
    size_t      keylen;

    if (!entry)
        return ;
    eq = ft_strchr(entry, '=');
    if (!eq)
    {
        ft_printf("declare -x %s\n", entry);
        return ;
    }
    keylen = (size_t)(eq - entry);
    write(1, "declare -x ", 11);
    write(1, entry, keylen);
    write(1, "=\"", 2);
    ft_printf("%s", eq + 1);
    write(1, "\"\n", 2);
}

/* Makes a deep copy, sorts it, and prints bash-like "declare -x ..." */
static void	print_export_sorted(char **env)
{
    char	**copy;
    int		i;

    copy = copy_env(env);
    if (!copy)
        return ;
    sort_env_copy(copy);
    i = 0;
    while (copy[i])
    {
        print_export_line(copy[i]);
        i++;
    }
    free_2d_array(copy);
}

/* -------------------------------------------------------------------------- */
/* Validate identifier and perform export mutation                            */
/* -------------------------------------------------------------------------- */
/*
** handle_export_arg (file-local)
** ------------------------------
** Validates arg's KEY (before '='); prints bash-like error if invalid.
**
** Return: int (0 ok, 1 on error/alloc failure)
*/
static int	handle_export_arg(char *arg, t_bash *bash)
{
	char	*eq;
	char	*name;

	eq = ft_strchr(arg, '=');
	if (eq)
		name = ft_substr(arg, 0, eq - arg);
	else
		name = ft_strdup(arg);
	if (!is_valid_identifier(name))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", arg);
		free(name);
		return (1);
	}
	else if (update_or_add_env(&bash->env, arg) != 0)
	{
		ft_putendl_fd("minishell: export: failed to update env",
			STDERR_FILENO);
		free(name);
		return (1);
	}
	free(name);
	return (0);
}

/* -------------------------------------------------------------------------- */
/* builtin: export                                                            */
/* -------------------------------------------------------------------------- */
/*
** ft_export
** ---------
** Behavior:
**  - No args: print sorted "declare -x KEY=VAL" listing.
**  - With args: for each KEY or KEY=VAL:
**      * invalid identifier -> error to stderr, cumulative status=1
**      * KEY                 -> no change (bash-compatible)
**      * KEY=VAL             -> add or replace
**
** Params:  argv : char**  (argv[0]=="export")
**          bash : t_bash*
** Return:  int  0 if all ok, 1 if any arg invalid/failed
*/
int	ft_export(char **argv, t_bash *bash)
{
	int	i;
	int	status;

	if (!argv[1])
	{
		print_export_sorted(bash->env);
		return (0);
	}
	i = 1;
	status = 0;
	while (argv[i])
	{
		if (handle_export_arg(argv[i], bash))
			status = 1;
		i++;
	}
	bash->exit_status = status;
	return (status);
}
