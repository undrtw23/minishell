/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzec <gkorzecz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/22 16:00:00 by gkorzec           #+#    #+#             */
/*   Updated: 2024/06/07 14:18:47 by gkorzec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*result;
	unsigned int	slen;

	if (s == NULL)
		return (NULL);
	slen = ft_strlen(s);
	i = 0;
	if (start >= slen)
		len = 0;
	else if (len > slen - start)
		len = slen - start;
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (result == NULL)
		return (NULL);
	while (i < len && start + i < slen)
	{
		result[i] = s[start + i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
