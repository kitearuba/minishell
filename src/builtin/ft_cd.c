/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:34:17 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Print a bash-like cd error line to stderr                                  */
/* -------------------------------------------------------------------------- */
/*
** cd_error (file-local)
** ---------------------
** Prints: "minishell: cd: <dest or (null)>: <strerror(errno)>\n"
**
** Params:  dest : path attempted (may be NULL/"")
*/
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

/* -------------------------------------------------------------------------- */
/* Update PWD and OLDPWD after successful chdir                               */
/* -------------------------------------------------------------------------- */
/*
** update_pwd_vars (file-local)
** ----------------------------
** Uses getcwd to compute new PWD, then env_set(OLDPWD) and env_set(PWD).
**
** Params:  bash   : shell state
**          oldpwd : previous working directory (string buffer)
*/
static void	update_pwd_vars(t_bash *bash, const char *oldpwd)
{
	char	newpwd[PATH_MAX];

	if (!getcwd(newpwd, sizeof(newpwd)))
		newpwd[0] = '\0';
	env_set(bash, "OLDPWD", oldpwd);
	env_set(bash, "PWD", newpwd);
}

/* -------------------------------------------------------------------------- */
/* Resolve cd target from argv or $HOME                                       */
/* -------------------------------------------------------------------------- */
/*
** resolve_dest (file-local)
** -------------------------
** If argv[1] missing, returns $HOME via ft_getenv; prints error if not set.
**
** Params:  bash : shell state (to read HOME)
**          argv : command argv
**          out  : [out] destination pointer (no allocation here)
** Return:  int  (0 ok, 1 error already reported & bash->exit_status set)
*/
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

/* -------------------------------------------------------------------------- */
/* builtin: cd                                                                */
/* -------------------------------------------------------------------------- */
/*
** ft_cd
** -----
** Behavior:
**  - "cd" with no args -> $HOME (error if not set, status 1)
**  - one arg -> chdir(arg)
**  - >1 arg -> error "too many arguments", status 1
** On success updates OLDPWD/PWD and sets status 0.
**
** Params:  argv : char**  (command arguments)
**          bash : t_bash* (state/env)
** Return:  int  exit status (0 on success, 1 on error)
*/
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
