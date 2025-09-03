/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:09 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:56:33 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Replaces the n-th string of the array *big with the strings from array small.
Allocates a new array, inserts all elements of small at position n,
copies other elements from *big, and frees the old array.
Returns the new array, or NULL on error. */

int	copy_until_replace(char **dst, char **src, int stop, int *pos)
{
	int	i;
	int	d;

	i = 0;
	d = *pos;
	while (src && src[i])
	{
		if (i == stop)
			break ;
		dst[d++] = ft_strdup(src[i]);
		i++;
	}
	*pos = d;
	return (i);
}

int	copy_small_array(char **dst, char **small, int *pos)
{
	int	i;
	int	d;

	i = 0;
	d = *pos;
	while (small && small[i])
	{
		dst[d++] = ft_strdup(small[i]);
		i++;
	}
	*pos = d;
	return (i);
}

void	copy_remaining(char **dst, char **src, int start, int *pos)
{
	int	i;
	int	d;

	i = start;
	d = *pos;
	while (src && src[i])
	{
		dst[d++] = ft_strdup(src[i]);
		i++;
	}
	*pos = d;
}

char	**ft_array_replace(char ***big, char **small, int n)
{
	char	**tmp;
	int		len_big;
	int		len_small;
	int		pos;

	if (!big || !*big || n < 0)
		return (NULL);
	len_big = ft_arr_len(*big);
	if (n > len_big)
		return (NULL);
	len_small = ft_arr_len(small);
	tmp = ft_calloc(len_big + len_small + 1, sizeof(char *));
	if (!tmp)
		return (put_err(NULL, "Malloc failed", 1, NULL));
	pos = 0;
	copy_until_replace(tmp, *big, n, &pos);
	copy_small_array(tmp, small, &pos);
	copy_remaining(tmp, *big, n + 1, &pos);
	free_array(big);
	*big = tmp;
	return (*big);
}
