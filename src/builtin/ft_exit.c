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

#include "../../include/minishell.h"

static int	is_numeric(const char *str)
{
	int	i;

	i = 0;
	if (!str || !*str)
		return (0);
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!ft_isdigit(str[i]))
		return (0);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	ft_exit(char **argv, t_bash *bash)
{
	long long	code;
	int			arg_count;

	arg_count = 0;
	while (argv[arg_count])
		arg_count++;
	ft_printf_fd(STDOUT_FILENO, "exit\n");
	if (arg_count == 1)
		free_all_and_exit(bash, bash->exit_status);
	if (!is_numeric(argv[1]))
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: exit: %s: numeric argument required\n", argv[1]);
		free_all_and_exit(bash, 255);
	}
	if (arg_count > 2)
	{
		ft_printf_fd(STDERR_FILENO,
			"minishell: exit: too many arguments\n");
		bash->exit_status = 1;
		return ;
	}
	code = ft_atoll(argv[1]);
	free_all_and_exit(bash, (unsigned char)code);
}
