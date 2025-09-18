/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelona.co> +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 23:25:00 by chrrodri          #+#    #+#             */
/*   Updated: 2025/08/07 05:45:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*   ft_exit.c                                                                 */
/*                                                                            */
/*   Behavior (bash-compatible):                                               */
/*   - exit           -> exit with current bash->exit_status                   */
/*   - exit <num>     -> exit with (unsigned char)num (wrap 0..255)            */
/*   - exit <bad>     -> print error, exit with 2                              */
/*   - exit <n> <m>   -> print "too many arguments", DO NOT exit, return 1     */
/*                                                                            */
/*   Notes for 42: keep helpers tiny, no dynamic alloc here.                   */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

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
		res = res * 10 + (*s - '0');
		s++;
	}
	return (res * sign);
}

int	ft_exit(char **argv, t_bash *bash)
{
	int			ac;
	long long	v;

	ac = 0;
	while (argv[ac])
		ac++;
	if (ac == 1)
	{
		ft_printf_fd(STDOUT_FILENO, "exit\n");
		free_all_and_exit(bash, bash->exit_status);
	}
	if (!is_num_str(argv[1]))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", argv[1]);
		ft_printf_fd(STDOUT_FILENO, "exit\n");
		free_all_and_exit(bash, 2);
	}
	if (ac > 2)
	{
		ft_printf_fd(STDERR_FILENO, "minishell: exit: too many arguments\n");
		bash->exit_status = 1;
		return (1); /* stay in shell */
	}
	v = atoll_s(argv[1]);
	ft_printf_fd(STDOUT_FILENO, "exit\n");
	free_all_and_exit(bash, (unsigned char)v);
	return (0);
}