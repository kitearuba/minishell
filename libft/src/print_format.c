/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_format.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/14 20:42:11 by chrrodri          #+#    #+#             */
/*   Updated: 2024/08/14 20:46:55 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	print_format(const char *ptr, va_list args, int fd)
{
	if (*ptr == 'c')
		return (handle_char(args, fd));
	else if (*ptr == 's')
		return (handle_string(args, fd));
	else if (*ptr == 'p')
		return (handle_pointer(args, fd));
	else if (*ptr == 'd' || *ptr == 'i')
		return (handle_int(args, fd));
	else if (*ptr == 'u')
		return (handle_uint(args, fd));
	else if (*ptr == 'x' || *ptr == 'X')
		return (handle_hex(args, fd, *ptr == 'X'));
	else if (*ptr == '%')
		return (handle_percent(fd));
	return (0);
}
