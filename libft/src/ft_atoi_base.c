/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 20:32:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/02/02 22:55:33 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

static int	get_digit_value(char c)
{
	if (c >= '0' && c <= '9')
		return (c - '0');
	if (c >= 'A' && c <= 'F')
		return (c - 'A' + 10);
	if (c >= 'a' && c <= 'f')
		return (c - 'a' + 10);
	return (-1);
}

static const char	*skip_whitespace_and_sign(const char *str, int *sign)
{
	while (*str == ' ' || *str == '\t' || *str == '\n'
		|| *str == '\v' || *str == '\f' || *str == '\r')
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			*sign = -1;
		str++;
	}
	return (str);
}

static int	handle_overflow(long result, int sign, int value, int base)
{
	if (result > (MAX_LONG - value) / base)
	{
		if (sign == 1)
			return (MAX_INT);
		return (MIN_INT);
	}
	return (-1);
}

int	ft_atoi_base(const char *str, int base)
{
	long	result;
	int		sign;
	int		value;
	int		overflow_check;

	if (!str || base < 2 || base > 16)
		return (0);
	sign = 1;
	str = skip_whitespace_and_sign(str, &sign);
	if (base == 16 && *str == '0' && (str[1] == 'x' || str[1] == 'X'))
		str += 2;
	result = 0;
	while (*str)
	{
		value = get_digit_value(*str);
		if (value == -1)
			break ;
		overflow_check = handle_overflow(result, sign, value, base);
		if (overflow_check != -1)
			return (overflow_check);
		result = result * base + value;
		str++;
	}
	return ((int)(result * sign));
}
