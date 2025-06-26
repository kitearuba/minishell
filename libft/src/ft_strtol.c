/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtol.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:32:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/02/04 21:55:07 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static int	ft_isspace(char c)
{
	if (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r')
		return (1);
	return (0);
}

static int	ft_isbase(char c, int base)
{
	if (ft_isdigit(c))
		return (c - '0' < base);
	if (base > 10 && (c >= 'a' && c <= 'f'))
		return (c - 'a' + 10 < base);
	if (base > 10 && (c >= 'A' && c <= 'F'))
		return (c - 'A' + 10 < base);
	return (0);
}

static int	sign_assign(char c)
{
	int	sign;

	sign = 1;
	if (c == '-' || c == '+')
	{
		if (c == '-')
			sign = -1;
	}
	return (sign);
}

long	ft_strtol(const char *str, char **endptr, int base)
{
	long	result;
	int		sign;

	while (ft_isspace(*str))
		str++;
	sign = sign_assign(*str);
	str++;
	result = 0;
	while (ft_isbase(*str, base))
	{
		if (ft_isdigit(*str))
			result = result * base + (*str - '0');
		else if (*str >= 'a' && *str <= 'f')
			result = result * base + (*str - 'a' + 10);
		else if (*str >= 'A' && *str <= 'F')
			result = result * base + (*str - 'A' + 10);
		str++;
	}
	if (endptr)
		*endptr = (char *)str;
	return (result * sign);
}
