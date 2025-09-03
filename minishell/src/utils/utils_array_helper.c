/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_array_helper.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:32:02 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:32:23 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*result;

	tmp = ft_strjoin(s1, s2);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

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
