/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/24 18:20:23 by gkorzec           #+#    #+#             */
/*   Updated: 2025/07/30 18:13:23 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_countwords(char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		flag;

	i = 0;
	j = 0;
	flag = 0;
	while (s[i] != '\0')
	{
		if (s[i] != c && flag == 0)
		{
			i++;
			j = j + 1;
			flag = 1;
		}
		else if (s[i] == c)
		{
			i++;
			flag = 0;
		}
		else
			i++;
	}
	return (j);
}

static size_t	ft_len(char const *s, char c, size_t j)
{
	size_t	incount;

	incount = 0;
	while (s[j] != c && s[j] != '\0')
	{
		incount ++;
		j++;
	}
	return (incount);
}

static char	*fill_str(char const *s, char c, size_t count)
{
	size_t	i;
	char	*new;

	i = 0;
	new = (char *)malloc(sizeof(char) * (ft_len(s, c, count) + 1));
	if (new == NULL)
		return (NULL);
	while (i < ft_len(s, c, count) && s[count + i] != '\0')
	{
		new[i] = s[count + i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

static void	free_str(char **result, size_t i)
{
	while (i > 0)
	{
		i--;
		free(result[i]);
	}
	free(result);
}

char	**ft_split(char const *s, char c)
{
	char	**result;
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	result = (char **)malloc(sizeof(char *) * (ft_countwords(s, c) + 1));
	if (result == NULL)
		return (NULL);
	while (i < ft_countwords(s, c))
	{
		while (s[count] == c)
			count++;
		result[i] = fill_str(s, c, count);
		if (result[i] == NULL)
		{
			free_str(result, i);
			return (NULL);
		}
		while (s[count] != c && s[count] != '\0')
			count++;
		i++;
	}
	result[i] = NULL;
	return (result);
}
