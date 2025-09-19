/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:43:48 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/19 13:01:30 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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

static void	print_export_sorted(char **env)
{
	char	**copy;
	int		i;

	copy = copy_env(env);
	if (!copy)
		return ;
	sort_env_copy(copy);
	i = -1;
	while (copy[++i])
		printf("declare -x %s\n", copy[i]);
	free_2d_array(copy);
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
