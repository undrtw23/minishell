/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:59:21 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/20 16:01:09 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

static size_t	print_memory(int fd, unsigned long address)
{
	const char		*hex_digits;
	unsigned long	ret;

	hex_digits = "0123456789abcdef";
	ret = 0;
	if (address >= 16)
		ret += print_memory(fd, (address / 16));
	ft_printchar_fd(fd, hex_digits[address % 16]);
	return (ret + 1);
}

int	ft_printptr_fd(int fd, unsigned long ptr)
{
	if (!ptr)
		return (ft_printstr_fd(fd, "(nil)"));
	return (ft_printstr_fd(fd, "0x") + print_memory(fd, ptr));
}
