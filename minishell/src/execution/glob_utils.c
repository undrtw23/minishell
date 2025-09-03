/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:09:21 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:18:59 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*ft_strjoin_char_flag(char *str, char c, int quoted)
{
	size_t	len;
	size_t	extra;
	char	*new_str;
	size_t	pos;

	if (str)
		len = ft_strlen(str);
	else
		len = 0;
	if (quoted)
		extra = 1;
	else
		extra = 0;
	new_str = (char *)malloc(len + 1 + extra + 1);
	if (!new_str)
		return (free(str), NULL);
	if (str)
		ft_memcpy(new_str, str, len);
	pos = len;
	if (quoted)
		new_str[pos++] = '\x01';
	new_str[pos++] = c;
	new_str[pos] = '\0';
	return (free(str), new_str);
}

char	*ft_strjoin_char(char *str, char c)
{
	char	*new_str;
	size_t	len;
	size_t	i;

	if (!str)
	{
		new_str = (char *)malloc(2 * sizeof(char));
		if (!new_str)
			return (NULL);
		new_str[0] = c;
		new_str[1] = '\0';
		return (new_str);
	}
	len = 0;
	while (str[len])
		len++;
	new_str = (char *)malloc((len + 2) * sizeof(char));
	if (!new_str)
		return (free(str), NULL);
	i = -1;
	while (++i < len)
		new_str[i] = str[i];
	new_str[len] = c;
	new_str[len + 1] = '\0';
	return (free(str), new_str);
}

char	*ft_strjoin_char_with_flag(char *str, char c, int quoted)
{
	char	*new_str;
	size_t	len;
	size_t	extra;
	size_t	pos;

	new_str = NULL;
	if (str)
		len = ft_strlen(str);
	else
		len = 0;
	if (quoted)
		extra = 1;
	else
		extra = 0;
	new_str = (char *)malloc(len + 2 + extra);
	if (!new_str)
		return (free(str), NULL);
	if (str)
		ft_memcpy(new_str, str, len);
	pos = len;
	if (quoted)
		new_str[pos++] = '\x01';
	new_str[pos++] = c;
	new_str[pos] = '\0';
	return (free(str), new_str);
}

char	*preprocess_glob(char *glob, int *quo)
{
	char	*result;
	char	*ptr;
	char	quote;

	result = ft_strdup("");
	ptr = glob;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	while (*ptr)
	{
		if (*ptr == '\'' || *ptr == '"')
		{
			quote = *ptr++;
			while (*ptr && *ptr != quote)
				result = ft_strjoin_char_flag(result, *ptr++, 1);
			if (*ptr == quote)
				ptr++;
		}
		else
			result = ft_strjoin_char_flag(result, *ptr++, 0);
	}
	return (result);
}

int	next_char(char **pattern, char *c, int *quoted)
{
	if (!*pattern || !**pattern)
		return (0);
	*quoted = 0;
	if (**pattern == '\x01')
	{
		*quoted = 1;
		(*pattern)++;
	}
	*c = *(*pattern)++;
	return (1);
}
