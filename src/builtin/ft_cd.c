/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/07 03:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/07 03:00:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

#include <errno.h>

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

int	ft_cd(char **argv, t_bash *bash)
{
    char	oldpwd[PATH_MAX];
    char	*dest;

    if (!getcwd(oldpwd, sizeof(oldpwd)))
        oldpwd[0] = '\0';
    if (!argv[1])
    {
        dest = ft_getenv(bash->env, "HOME");
        if (!dest || !*dest)
        {
            ft_printf_fd(STDERR_FILENO, "minishell: cd: HOME not set\n");
            bash->exit_status = 1;
            return (1);
        }
    }
    else
        dest = argv[1];
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