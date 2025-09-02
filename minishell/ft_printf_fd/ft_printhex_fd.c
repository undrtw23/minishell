/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printhex_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:56:08 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/20 15:57:01 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

int	ft_printhex_fd(int fd, unsigned int n, char minmaj)
{
	char	*hex;
	int		ret;

	ret = 0;
	if (minmaj == 'm')
		hex = "0123456789abcdef";
	else if (minmaj == 'M')
		hex = "0123456789ABCDEF";
	else
		return (0);
	if (n >= 16)
		ret += ft_printhex_fd(fd, (n / 16), minmaj);
	ft_printchar_fd(fd, hex[n % 16]);
	ret++;
	return (ret);
}
