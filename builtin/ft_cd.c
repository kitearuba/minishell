/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/10/03 17:50:52 by bsamy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_cd_args(char **argv, t_bash *bash)
{
	if (argv[1] && argv[2])
	{
		ft_printf_fd(STDERR_FILENO, "minishell: cd: too many arguments\n");
		bash->exit_status = 1;
		return (1);
	}
	return (0);
}

static int	try_change_directory(t_bash *bash, char **argv,
		char oldpwd[PATH_MAX], int *is_dash)
{
	char	*dest;

	if (!getcwd(oldpwd, PATH_MAX))
		oldpwd[0] = '\0';
	if (resolve_dest(bash, argv, &dest, is_dash))
		return (1);
	if (chdir(dest) == -1)
	{
		cd_error(dest);
		bash->exit_status = 1;
		return (1);
	}
	return (0);
}

static void	handle_post_cd(t_bash *bash, char oldpwd[PATH_MAX], int is_dash)
{
	char	*pwd_now;

	update_pwd_vars(bash, oldpwd);
	if (is_dash)
	{
		pwd_now = ft_getenv(bash->env, "PWD");
		if (pwd_now && *pwd_now)
			ft_printf_fd(STDOUT_FILENO, "%s\n", pwd_now);
	}
	bash->exit_status = 0;
}

int	ft_cd(char **argv, t_bash *bash)
{
	char	oldpwd[PATH_MAX];
	int		is_dash;

	if (check_cd_args(argv, bash))
		return (1);
	if (try_change_directory(bash, argv, oldpwd, &is_dash))
		return (1);
	handle_post_cd(bash, oldpwd, is_dash);
	return (0);
}
