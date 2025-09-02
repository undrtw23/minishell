/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printunsi_fd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:03:37 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/20 16:06:21 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

int	ft_printunsi_fd(int fd, unsigned int u)
{
	int		ret;

	ret = 0;
	if (u >= 10)
		ret += ft_printnbr_fd(fd, (u / 10));
	ft_printchar_fd(fd, ((u % 10) + '0'));
	ret++;
	return (ret);
}
