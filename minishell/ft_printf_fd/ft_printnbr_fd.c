/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printnbr_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:57:37 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/20 15:58:54 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

int	ft_printnbr_fd(int fd, int n)
{
	int		ret;

	ret = 0;
	if (n == -2147483648)
	{
		write(fd, "-2147483648", 11);
		return (11);
	}
	if (n < 0)
	{
		n = -n;
		ft_printchar_fd(fd, '-');
		ret++;
	}
	if (n >= 10)
		ret += ft_printnbr_fd(fd, (n / 10));
	ft_printchar_fd(fd, ((n % 10) + '0'));
	ret++;
	return (ret);
}
