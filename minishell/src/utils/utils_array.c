/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:42:09 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/08/27 23:26:30 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Returns the number of elements (rows) in a NULL-terminated string array m.
If m is NULL, returns 0. */
int	ft_arr_len(char **m)
{
	int	i;

	i = 0;
	if (!m)
		return (i);
	while (m[i])
		i++;
	return (i);
}

/* Creates a copy of a NULL-terminated string array arr_ptr.
Allocates new memory for each string and for the array itself.
Basically strdup for **str.*/
char	**ft_dup_array(char **arr_ptr)
{
	char	**arr_copy;
	int		n_rows;
	int		i;

	i = 0;
	n_rows = ft_arr_len(arr_ptr);
	arr_copy = malloc(sizeof(char *) * (n_rows + 1));
	if (!arr_copy)
		return (NULL);
	while (arr_ptr[i])
	{
		arr_copy[i] = ft_strdup(arr_ptr[i]);
		if (!arr_copy[i])
		{
			free_array(&arr_copy);
			return (NULL);
		}
		i++;
	}
	arr_copy[i] = NULL;
	return (arr_copy);
}

/* Returns a new array with newstr appended to the input array in.
Frees the original array in. If newstr is NULL, returns in unchanged.
Use for append to env.*/
char	**ft_array_insert(char **in, char *newstr)
{
	char	**out;
	int		len;
	int		i;

	i = -1;
	out = NULL;
	if (!newstr)
		return (in);
	len = ft_arr_len(in);
	out = ft_calloc(1, sizeof(char *) * (len + 2));
	if (!out)
		return (put_err(NULL, "Malloc failed", 1, NULL));
	out[len + 1] = NULL;
	while (++i < len)
	{
		out[i] = ft_strdup(in[i]);
		if (!out[i])
		{
			free_array(&in);
			free_array(&out);
		}
	}
	out[i] = ft_strdup(newstr);
	free_array(&in);
	return (out);
}

/* Replaces the n-th string of the array *big with the strings from array small.
Allocates a new array, inserts all elements of small at position n,
copies other elements from *big, and frees the old array.
Returns the new array, or NULL on error. */
char	**ft_array_replace(char ***big, char **small, int n)
{
	char	**tmpstr;
	int		i[3];
	int		l;

	l = ft_arr_len(*big);
	i[0] = -1;
	i[1] = -1;
	i[2] = -1;
	if (!big || !*big || n < 0 || n > l)
		return (NULL);
	tmpstr = ft_calloc(l + ft_arr_len(small) + 1, sizeof(char *));
	if (!tmpstr)
		return (put_err(NULL, "Malloc failed", 1, NULL));
	while (++i[0] < l)
	{
		if (i[0] != n)
			tmpstr[++i[2]] = ft_strdup(big[0][i[0]]);
		else
		{
			while (small && small[++i[1]])
				tmpstr[++i[2]] = ft_strdup(small[i[1]]);
		}
	}
	if (n == l)
	{
		i[1] = -1;
		while (small && small[++i[1]])
			tmpstr[++i[2]] = ft_strdup(small[i[1]]);
	}
	free_array(big);
	*big = tmpstr;
	return (*big);
}
