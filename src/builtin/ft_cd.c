/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 10:59:42 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cd_error(const char *dest)
{
	const char	*msg;

	msg = strerror(errno);
	ft_printf_fd(STDERR_FILENO, "minishell: cd: ");
	if (dest && *dest)
		ft_printf_fd(STDERR_FILENO, dest);
	else
		ft_printf_fd(STDERR_FILENO, "(null)");
	ft_printf_fd(STDERR_FILENO, ": ");
	ft_printf_fd(STDERR_FILENO, msg);
	ft_printf_fd(STDERR_FILENO, "\n");
}

static void	update_pwd_vars(t_bash *bash, const char *oldpwd)
{
	char	newpwd[PATH_MAX];

	if (!getcwd(newpwd, sizeof(newpwd)))
		newpwd[0] = '\0';
	env_set(bash, "OLDPWD", oldpwd);
	env_set(bash, "PWD", newpwd);
}

static int	resolve_dest(t_bash *bash, char **argv, char **out)
{
	char	*home;

	if (!argv[1])
	{
		home = ft_getenv(bash->env, "HOME");
		if (!home || !*home)
		{
			ft_printf_fd(STDERR_FILENO,
				"minishell: cd: HOME not set\n");
			bash->exit_status = 1;
			return (1);
		}
		*out = home;
		return (0);
	}
	*out = argv[1];
	return (0);
}

int	ft_cd(char **argv, t_bash *bash)
{
	char	oldpwd[PATH_MAX];
	char	*dest;

	if (argv[1] && argv[2])
	{
		ft_printf_fd(STDERR_FILENO, "minishell: cd: too many arguments\n");
		bash->exit_status = 1;
		return (1);
	}
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	if (resolve_dest(bash, argv, &dest))
		return (1);
	if (chdir(dest) == -1)
	{
		cd_error(dest);
		bash->exit_status = 1;
		return (1);
	}
	update_pwd_vars(bash, oldpwd);
	bash->exit_status = 0;
	return (0);
}
