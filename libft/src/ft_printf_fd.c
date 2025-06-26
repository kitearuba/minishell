/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/10 21:19:50 by chrrodri          #+#    #+#             */
/*   Updated: 2024/08/14 20:48:05 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_printf_fd(int fd, const char *format, ...)
{
	va_list	args;
	int		count;

	count = 0;
	va_start(args, format);
	count = handle_format(format, args, fd);
	va_end(args);
	return (count);
}

int	handle_format(const char *format, va_list args, int fd)
{
	const char	*ptr;
	int			count;

	count = 0;
	ptr = format;
	while (*ptr)
	{
		if (*ptr == '%')
		{
			ptr++;
			count += print_format(ptr, args, fd);
		}
		else
		{
			ft_putchar_fd(*ptr, fd);
			count++;
		}
		ptr++;
	}
	return (count);
}
