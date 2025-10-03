/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:43:48 by chrrodri          #+#    #+#             */
/*   Updated: 2025/10/03 17:28:10 by bsamy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	update_or_add_env(char ***env, const char *arg)
{
	int		i;
	char	*name;
	char	*eq;

	eq = ft_strchr(arg, '=');
	if (!eq)
		return (0);
	name = ft_substr(arg, 0, eq - arg);
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

static int	handle_export_arg(char *arg, t_bash *bash)
{
	char	*eq;
	char	*name;

	eq = ft_strchr(arg, '=');
	if (eq)
		name = ft_substr(arg, 0, eq - arg);
	else
		name = ft_strdup(arg);
	if (ft_error_handling(eq, name, bash, arg))
		return (0);
	if (update_or_add_env(&bash->env, arg))
	{
		ft_putendl_fd("minishell: export: failed to update env", STDERR_FILENO);
		free(name);
		return (1);
	}
	export_list_remove(bash, name);
	return (0);
}

int	ft_error_handling(char *eq, char *name, t_bash *bash, char *arg)
{
	if (!is_valid_identifier(name))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: export: `%s': not a valid identifier\n", arg);
		free(name);
		return (1);
	}
	if (!eq)
	{
		if (export_declare_name_only(bash, name))
			return (free(name), 1);
		return (free(name), 0);
	}
	if (!name)
		return (1);
	return (0);
}

int	ft_export(char **argv, t_bash *bash)
{
	int	i;
	int	status;

	if (!argv[1])
		return (print_export_sorted(bash), 0);
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
