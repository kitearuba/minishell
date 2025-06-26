/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pointer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:52:50 by chrrodri          #+#    #+#             */
/*   Updated: 2024/08/14 20:53:38 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	handle_pointer(va_list args, int fd)
{
	unsigned long	ptr_value;
	int				len;

	ptr_value = (unsigned long)va_arg(args, void *);
	if (ptr_value == 0)
	{
		ft_putstr_fd("(nil)", fd);
		return (5);
	}
	ft_putstr_fd("0x", fd);
	ft_puthex(ptr_value, 0, fd);
	len = 2 + get_hex_length(ptr_value);
	return (len);
}
