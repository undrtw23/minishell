/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_matching2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:09:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:19:06 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*find_char_in_str(char *str, char target)
{
	while (*str)
	{
		if (*str == target)
			return (str);
		str++;
	}
	return (NULL);
}

int	match_with_wildcard(char *pattern, char *text)
{
	pattern = skip_stars(pattern);
	if (!*pattern)
		return (1);
	while (*text)
	{
		if (match_at_position(pattern, text))
			return (1);
		text++;
	}
	return (0);
}

int	simple_glob_match(char *pattern, char *text)
{
	while (*pattern && *text)
	{
		if (*pattern == '*')
			return (match_with_wildcard(pattern, text));
		if (*pattern == '\x01')
			pattern++;
		if (*pattern != *text)
			return (0);
		pattern++;
		text++;
	}
	pattern = skip_stars(pattern);
	return (*pattern == '\0');
}

int	match_glob(char *glob, char *file)
{
	char	*processed_pattern;
	int		result;

	processed_pattern = preprocess_glob(glob, (int []){0, 0, 0});
	result = simple_glob_match(processed_pattern, file);
	free(processed_pattern);
	return (result);
}

int	should_include_file(char *filename, char *glob)
{
	int	allows_dot_files;

	allows_dot_files = check_dot_file_pattern(glob);
	if (filename[0] == '.' && !allows_dot_files)
		return (0);
	return (match_glob(glob, filename));
}
