/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzec <gkorzecz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 13:54:36 by gkorzec           #+#    #+#             */
/*   Updated: 2024/05/30 13:43:42 by gkorzec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	test_set(char c, char const *set)
{
	int	i;

	i = 0;
	while (set[i] != '\0')
	{
		if (c == set[i])
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		sta;
	int		end;
	int		i;
	char	*s;

	sta = 0;
	end = ft_strlen(s1) - 1;
	i = 0;
	while (s1[sta] != '\0' && test_set(s1[sta], set) != 0)
		sta++;
	while (end >= sta && test_set(s1[end], set) != 0)
		end--;
	s = (char *)malloc(sizeof(char) * (end - sta + 2));
	if (s == NULL)
		return (NULL);
	while (sta <= end)
	{
		s[i] = s1[sta];
		sta++;
		i++;
	}
	s[i] = '\0';
	return (s);
}
