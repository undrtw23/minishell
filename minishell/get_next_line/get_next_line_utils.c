/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gkorzec <gkorzecz@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:54:41 by gkorzec           #+#    #+#             */
/*   Updated: 2024/07/30 12:51:26 by gkorzec          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	ft_strlen_gnl(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}

char	*ft_strchr_gnl(const char *s, int c)
{
	int	i;

	i = 0;
	if ((char)c == '\0')
		return ((char *)&s[ft_strlen_gnl(s)]);
	while (s[i] != '\0')
	{
		if ((char)c == s[i])
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}

char	*ft_substr_gnl(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	char			*result;
	unsigned int	slen;

	slen = ft_strlen_gnl(s);
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

char	*ft_strdup_gnl(const char *s)
{
	int		i;
	char	*sdup;

	i = 0;
	sdup = (char *)malloc((sizeof(char) * ft_strlen_gnl(s)) + 1);
	if (sdup == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		sdup[i] = s[i];
		i++;
	}
	sdup[i] = '\0';
	return (sdup);
}

char	*ft_strjoin_gnl(char const *s1, char const *s2)
{
	int		totalen;
	int		i;
	int		j;
	char	*s;

	i = 0;
	j = 0;
	totalen = ft_strlen_gnl(s1) + ft_strlen_gnl(s2) + 1;
	s = (char *)malloc(sizeof(char) * totalen);
	if (s == NULL)
		return (NULL);
	while (s1[i] != '\0')
	{
		s[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		s[i + j] = s2[j];
		j++;
	}
	s[i + j] = '\0';
	return (s);
}
