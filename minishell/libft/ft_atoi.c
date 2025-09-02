/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzec <gkorzecz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:03:04 by gkorzec           #+#    #+#             */
/*   Updated: 2024/05/20 16:29:22 by gkorzec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	int	i;
	int	sign;
	int	conv;

	i = 0;
	sign = 1;
	conv = 0;
	while ((nptr[i] >= '\t' && nptr[i] <= '\r') || nptr[i] == ' ')
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -1 * sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		conv = conv * 10 + (nptr[i] - '0');
		i++;
	}
	conv = sign * conv;
	return (conv);
}
