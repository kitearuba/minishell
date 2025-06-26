/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 21:25:26 by chrrodri          #+#    #+#             */
/*   Updated: 2024/08/10 21:27:00 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_putunbr(unsigned int n, int fd)
{
	if (n > 9)
		ft_putunbr(n / 10, fd);
	ft_putchar_fd((n % 10) + '0', fd);
}
