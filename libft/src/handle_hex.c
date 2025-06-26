/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:50:20 by chrrodri          #+#    #+#             */
/*   Updated: 2024/08/14 20:50:54 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	handle_hex(va_list args, int fd, int uppercase)
{
	unsigned int	num;

	num = va_arg(args, unsigned int);
	ft_puthex(num, uppercase, fd);
	return (get_hex_length(num));
}
