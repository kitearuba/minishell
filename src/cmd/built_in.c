/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:15:32 by chrrodri          #+#    #+#             */
/*   Updated: 2025/05/20 16:43:48 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int run_builtin(char **argv, t_bash *bash)
{
    if (ft_strcmp(argv[0], "echo") == 0)
        return (ft_echo(argv, bash));
    else if (ft_strcmp(argv[0], "cd") == 0)
        return (ft_cd(argv, bash));
    else if (ft_strcmp(argv[0], "pwd") == 0)
        return (ft_pwd(argv, bash));
    else if (ft_strcmp(argv[0], "export") == 0)
        return (ft_export(argv, bash));
    else if (ft_strcmp(argv[0], "unset") == 0)
        return (ft_unset(argv, bash));
    else if (ft_strcmp(argv[0], "env") == 0)
        return (ft_env(argv, bash));
    else if (ft_strcmp(argv[0], "exit") == 0)
        ft_exit(argv, bash);
    return (1);
}

int	is_builtin(const char *cmd)
{
    if (!cmd)
        return (0);
    return (!ft_strcmp(cmd, "cd")
        || !ft_strcmp(cmd, "echo")
        || !ft_strcmp(cmd, "pwd")
        || !ft_strcmp(cmd, "export")
        || !ft_strcmp(cmd, "unset")
        || !ft_strcmp(cmd, "env")
        || !ft_strcmp(cmd, "exit"));
}