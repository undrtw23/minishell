/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzec <gkorzecz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 15:57:29 by gkorzec           #+#    #+#             */
/*   Updated: 2024/05/31 12:33:58 by gkorzec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	intlen(int n)
{
	int		len;
	long	nbr;

	len = 0;
	nbr = n;
	if (nbr <= 0)
	{
		len++;
		nbr = -nbr;
	}
	while (nbr != 0)
	{
		len++;
		nbr = nbr / 10;
	}
	return (len);
}

static char	*ft_write(char *s, int i, long nbr)
{
	if (nbr < 0)
	{
		s[0] = '-';
		nbr = -nbr;
	}
	while (i >= 0)
	{
		if (s[i] == '-')
			break ;
		s[i] = (nbr % 10) + '0';
		nbr = nbr / 10;
		i--;
	}
	return (s);
}

char	*ft_itoa(int n)
{
	int		i;
	char	*s;
	long	nbr;

	nbr = n;
	i = intlen(n) - 1;
	s = (char *)malloc((sizeof(char) * i) + 2);
	if (!s)
		return (NULL);
	ft_memset(s, 0, i + 2);
	ft_write(s, i, nbr);
	return (s);
}
