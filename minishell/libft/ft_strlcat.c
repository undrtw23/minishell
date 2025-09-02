/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzec <gkorzecz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 16:15:39 by gkorzec           #+#    #+#             */
/*   Updated: 2024/05/28 19:23:08 by gkorzec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t siz)
{
	size_t	i;
	size_t	dstlen;
	size_t	srclen;

	i = 0;
	dstlen = ft_strlen(dst);
	srclen = ft_strlen(src);
	if (siz <= dstlen)
		return (srclen + siz);
	while ((dstlen + i) < (siz - 1) && src[i] != '\0')
	{
		dst[dstlen + i] = src[i];
		i++;
	}
	dst[i + dstlen] = '\0';
	return (srclen + dstlen);
}
