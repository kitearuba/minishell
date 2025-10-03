/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 12:00:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/07 07:01:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/* -------------------------------------------------------------------------- */
/* Test for -n (accepts -n, -nnn, ...)                                        */
/* -------------------------------------------------------------------------- */
/*
** is_n_flag (file-local)
** ----------------------
** Returns 1 if arg is '-n' with any number of trailing 'n's, else 0.
**
** Params:  arg : candidate flag string
** Return:  int
*/
static int	is_n_flag(char *arg)
{
	int	i;

	if (!arg || arg[0] != '-' || arg[1] != 'n')
		return (0);
	i = 2;
	while (arg[i] == 'n')
		i++;
	return (arg[i] == '\0');
}

/* -------------------------------------------------------------------------- */
/* builtin: echo                                                              */
/* -------------------------------------------------------------------------- */
/*
** ft_echo
** -------
** Prints args separated by a single space. If one or more -n flags are
** present, suppresses the trailing newline.
**
** Params:  argv : char**  (argv[0]=="echo")
**          bash : t_bash* (sets exit_status=0)
** Return:  int  always 0
*/
int	ft_echo(char **argv, t_bash *bash)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (argv[i] && is_n_flag(argv[i]))
	{
		newline = 0;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], STDOUT_FILENO);
		if (argv[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (newline)
		ft_putchar_fd('\n', STDOUT_FILENO);
	bash->exit_status = 0;
	return (0);
}
