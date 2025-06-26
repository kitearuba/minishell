/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strappend.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chrrodri <chrrodri@student.42barcelon      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 16:06:31 by chrrodri          #+#    #+#             */
/*   Updated: 2024/08/14 20:08:15 by chrrodri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

void	ft_strappend(char *dst, const char *src1, const char *src2, size_t size)
{
	while (*src1 && --size)
		*dst++ = *src1++;
	if (src2 && size > 0)
		while (*src2 && --size)
			*dst++ = *src2++;
	*dst = '\0';
}
