/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoll.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:32:43 by chrrodri          #+#    #+#             */
/*   Updated: 2025/07/18 22:55:33 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

long long	ft_atoll(const char *str)
{
    long long	res;
    int			sign;

    res = 0;
    sign = 1;
    while (*str == ' ' || (*str >= 9 && *str <= 13))
        str++;
    if (*str == '-' || *str == '+')
    {
        if (*str == '-')
            sign = -1;
        str++;
    }
    while (*str && *str >= '0' && *str <= '9')
    {
        if (res > (LLONG_MAX - (*str - '0')) / 10)
        {
            if (sign == 1)
                return (LLONG_MAX);
            return (LLONG_MIN);
        }
        res = res * 10 + (*str - '0');
        str++;
    }
    return (res * sign);
}
