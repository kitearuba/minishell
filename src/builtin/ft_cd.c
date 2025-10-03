/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/10/03 15:11:39 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
** Print: minishell: cd: <dest or (null)>: <strerror(errno)>
*/
static void	cd_error(const char *dest)
{
	const char	*msg;

	msg = strerror(errno);
	ft_printf_fd(STDERR_FILENO, "minishell: cd: ");
	if (dest && *dest)
		ft_printf_fd(STDERR_FILENO, "%s", dest);
	else
		ft_printf_fd(STDERR_FILENO, "%s", "(null)");
	ft_printf_fd(STDERR_FILENO, ": %s\n", msg);
}

/*
** After successful chdir, refresh OLDPWD and PWD from getcwd.
*/
static void	update_pwd_vars(t_bash *bash, const char *oldpwd)
{
	char	newpwd[PATH_MAX];

	if (!getcwd(newpwd, sizeof(newpwd)))
		newpwd[0] = '\0';
	env_set(bash, "OLDPWD", oldpwd);
	env_set(bash, "PWD", newpwd);
}

/*
** Resolve destination:
**  - no arg -> $HOME (error if unset)
**  - "-"    -> $OLDPWD and set *is_dash = 1 (to print new cwd)
**  - other  -> argv[1]
** returns 0 on success, 1 on error already printed.
*/
static int	resolve_dest(t_bash *bash, char **argv, char **out, int *is_dash)
{
	char	*val;

	*is_dash = 0;
	if (!argv[1])
	{
		val = ft_getenv(bash->env, "HOME");
		if (!val || !*val)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: cd: HOME not set\n");
			bash->exit_status = 1;
			return (1);
		}
		*out = val;
		return (0);
	}
	if (argv[1][0] == '-' && argv[1][1] == '\0')
	{
		val = ft_getenv(bash->env, "OLDPWD");
		if (!val || !*val)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
			bash->exit_status = 1;
			return (1);
		}
		*out = val;
		*is_dash = 1;
		return (0);
	}
	*out = argv[1];
	return (0);
}

/*
** cd builtin:
**  - handles "cd", "cd -", one arg, too many args.
**  - updates PWD/OLDPWD. prints new cwd on "cd -" (from env PWD).
*/
int	ft_cd(char **argv, t_bash *bash)
{
	char	oldpwd[PATH_MAX];
	char	*dest;
	char	*pwd_now;
	int		is_dash;

	if (argv[1] && argv[2])
	{
		ft_printf_fd(STDERR_FILENO, "minishell: cd: too many arguments\n");
		bash->exit_status = 1;
		return (1);
	}
	if (!getcwd(oldpwd, sizeof(oldpwd)))
		oldpwd[0] = '\0';
	if (resolve_dest(bash, argv, &dest, &is_dash))
		return (1);
	if (chdir(dest) == -1)
	{
		cd_error(dest);
		bash->exit_status = 1;
		return (1);
	}
	update_pwd_vars(bash, oldpwd);
	if (is_dash)
	{
		pwd_now = ft_getenv(bash->env, "PWD");
		if (pwd_now && *pwd_now)
			ft_printf_fd(STDOUT_FILENO, "%s\n", pwd_now);
	}
	bash->exit_status = 0;
	return (0);
}
