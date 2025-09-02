/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printstr_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 16:01:27 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/04/20 16:03:20 by gkorzecz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf_fd.h"

int	ft_printstr_fd(int fd, char *s)
{
	int	i;

	i = 0;
	if (s == NULL)
	{
		ft_printstr_fd(fd, "(null)");
		return (6);
	}
	while (s[i] != '\0')
	{
		write(fd, &s[i], 1);
		i++;
	}
	return (i);
}
