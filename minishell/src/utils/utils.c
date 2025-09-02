/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:43:09 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/08/18 19:52:30 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Used in read_heredoc to check if heredoc delimiter (ex EOF)
has any quotes in it, "" doesnt expand $VARIABLES.*/
int	chrset_idx(const char *s, char *set)
{
	int	i;

	i = 0;
	if (!s)
		return (-1);
	while (s[i] != '\0')
	{
		if (ft_strchr(set, s[i]))
			return (i);
		i++;
	}
	return (-1);
}

/* Searches for the first occurrence of character 'c' in the string 's'.
Returns the index of the character if found.
If the character is '\0', returns the length of the string.
Returns -1 if 's' is NULL or if 'c' is not found in the string. */
int	char_index(const char *s, int c)
{
	unsigned char	c_unsigned;
	int				i;

	i = 0;
	if (!s)
		return (-1);
	c_unsigned = (unsigned char)c;
	while (s[i])
	{
		if (s[i] == c_unsigned)
			return (i);
		i++;
	}
	if (c_unsigned == '\0')
		return (i);
	return (-1);
}
