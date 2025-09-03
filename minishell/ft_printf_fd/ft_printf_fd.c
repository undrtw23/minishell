/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:48:59 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:47:33 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

static int	ft_putall(int fd, const char *format, size_t index, va_list args)
{
	if (format[index] == 's')
		return (ft_printstr_fd(fd, va_arg(args, char *)));
	else if (format[index] == 'c')
		return (ft_printchar_fd(fd, va_arg(args, int)));
	else if (format[index] == '%')
		return (ft_printchar_fd(fd, '%'));
	else if (format[index] == 'x')
		return (ft_printhex_fd(fd, va_arg(args, unsigned int), 'm'));
	else if (format[index] == 'X')
		return (ft_printhex_fd(fd, va_arg(args, unsigned int), 'M'));
	else if (format[index] == 'd' || format[index] == 'i')
		return (ft_printnbr_fd(fd, va_arg(args, int)));
	else if (format[index] == 'u')
		return (ft_printunsi_fd(fd, va_arg(args, unsigned int)));
	else if (format[index] == 'p')
		return (ft_printptr_fd(fd, va_arg(args, unsigned long)));
	return (-1);
}

static int	test_argument(const char *format, size_t index)
{
	char	*arguments;
	size_t	j;

	arguments = "scxXdupi%";
	j = 0;
	while (arguments[j] != '\0')
	{
		if (arguments[j] == format[index])
			return (1);
		j++;
	}
	return (-1);
}

int	ft_printf_fd(int fd, const char *format, ...)
{
	va_list	args;
	size_t	index;
	int		ret;

	ret = 0;
	index = 0;
	if (format == NULL)
		return (-1);
	va_start(args, format);
	while (format[index] != '\0')
	{
		if (format[index] != '%' && format[index] != '\0')
			ret += ft_printchar_fd(fd, format[index]);
		else if (format[index++] == '%')
		{
			if (test_argument(format, index) == -1)
				return (-1);
			ret += ft_putall(fd, format, index, args);
		}
		index++;
	}
	va_end(args);
	return (ret);
}
