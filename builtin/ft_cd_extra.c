/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_extra.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsamy <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/03 17:51:15 by bsamy             #+#    #+#             */
/*   Updated: 2025/10/03 17:51:55 by bsamy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
/*
** Print: minishell: cd: <dest or (null)>: <strerror(errno)>
*/
void	cd_error(const char *dest)
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
void	update_pwd_vars(t_bash *bash, const char *oldpwd)
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
int	get_home_or_oldpwd(t_bash *bash,
		char **out, int *is_dash, int is_oldpwd)
{
	char	*val;

	if (is_oldpwd)
	{
		val = ft_getenv(bash->env, "OLDPWD");
		if (!val || !*val)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: cd: OLDPWD not set\n");
			bash->exit_status = 1;
			return (1);
		}
		*is_dash = 1;
	}
	else
	{
		val = ft_getenv(bash->env, "HOME");
		if (!val || !*val)
		{
			ft_printf_fd(STDERR_FILENO, "minishell: cd: HOME not set\n");
			bash->exit_status = 1;
			return (1);
		}
	}
	*out = val;
	return (0);
}

int	resolve_dest(t_bash *bash, char **argv, char **out, int *is_dash)
{
	*is_dash = 0;
	if (!argv[1])
		return (get_home_or_oldpwd(bash, out, is_dash, 0));
	if (argv[1][0] == '-' && argv[1][1] == '\0')
		return (get_home_or_oldpwd(bash, out, is_dash, 1));
	*out = argv[1];
	return (0);
}
