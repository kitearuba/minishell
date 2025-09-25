/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 19:03:58 by chrrodri          #+#    #+#             */
/*   Updated: 2025/09/25 15:06:59 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/* -------------------------------------------------------------------------- */
/* Validate [+|-]?[0-9]+                                                      */
/* -------------------------------------------------------------------------- */
/*
** is_num_str (file-local)
** -----------------------
** Returns 1 if s is a valid signed decimal integer, else 0.
*/
static int	is_num_str(const char *s)
{
	int	i;

	if (!s || !*s)
		return (0);
	i = 0;
	if (s[i] == '+' || s[i] == '-')
		i++;
	if (!s[i])
		return (0);
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

/* -------------------------------------------------------------------------- */
/* Simple ASCII -> long long (no overflow checks)                             */
/* -------------------------------------------------------------------------- */
/*
** atoll_s (file-local)
** --------------------
** Parses signed decimal into long long. UB on overflow (OK for minishell).
*/
static long long	atoll_s(const char *s)
{
	long long	sign;
	long long	res;

	sign = 1;
	res = 0;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		res = (res * 10) + (*s - '0');
		s++;
	}
	return (res * sign);
}

/* -------------------------------------------------------------------------- */
/* Count argv items                                                           */
/* -------------------------------------------------------------------------- */
static int	args_count(char **argv)
{
	int	i;

	i = 0;
	while (argv && argv[i])
		i++;
	return (i);
}

/* -------------------------------------------------------------------------- */
/* Print "exit" and terminate via free_all_and_exit                           */
/* -------------------------------------------------------------------------- */
static void	exit_shell(t_bash *bash, int status)
{
	ft_printf_fd(STDOUT_FILENO, "exit\n");
	free_all_and_exit(bash, status);
}

/* -------------------------------------------------------------------------- */
/* builtin: exit                                                              */
/* -------------------------------------------------------------------------- */
/*
** ft_exit
** -------
** Behavior (bash-compatible):
**  - no args        : print "exit", exit with current $?.
**  - 1 numeric arg  : print "exit", exit with (unsigned char)value.
**  - 1 non-numeric  : print error, exit with 2.
**  - >=2 args and argv[1] numeric: print error "too many arguments", do not
**    exit, set status=1 and return 1.
**
** Params:  argv : char** (argv[0]=="exit")
**          bash : t_bash*
** Return:  int  only returns on "too many arguments" path (1), else no return.
*/
int	ft_exit(char **argv, t_bash *bash)
{
	int			ac;
	long long	v;

	ac = args_count(argv);
	if (ac == 1)
		exit_shell(bash, bash->exit_status);
	if (!is_num_str(argv[1]))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: exit: %s: "
			"numeric argument required\n", argv[1]);
		exit_shell(bash, 2);
	}
	if (ac > 2)
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: exit: too many arguments\n");
		bash->exit_status = 1;
		return (1);
	}
	v = atoll_s(argv[1]);
	exit_shell(bash, (unsigned char)v);
	return (0);
}
