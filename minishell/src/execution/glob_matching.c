/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob_matching.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:09:09 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:11:13 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	match_at_position(char *pattern, char *text)
{
	char	pc;
	int		quoted;

	while (*text)
	{
		if (!next_char(&pattern, &pc, &quoted))
			return (*text == '\0');
		if (pc == '*' && !quoted)
			return (match_with_wildcard(pattern, text));
		if (pc != *text)
			return (0);
		text++;
	}
	while (next_char(&pattern, &pc, &quoted))
	{
		if (pc != '*' || quoted)
			return (0);
	}
	return (1);
}

int	find_glob(char *token, int *quo)
{
	while (*token)
	{
		upd_quo(quo, *token);
		if (!quo[2] && *token == '*')
			return (1);
		token++;
	}
	return (0);
}

char	*advance_str_past_q(char *headglob, int *quo, int prev_q_state)
{
	while (*headglob && (*headglob == '\'' || *headglob == '\"'))
	{
		prev_q_state = quo[2];
		upd_quo(quo, *headglob);
		headglob++;
	}
	return (headglob);
}

int	check_dot_file_pattern(char *original_glob)
{
	char	*ptr;

	ptr = original_glob;
	if (*ptr == '.')
		return (1);
	if (*ptr == '"' && *(ptr + 1) == '.' && *(ptr + 2) == '"')
		return (1);
	if (*ptr == '\'' && *(ptr + 1) == '.' && *(ptr + 2) == '\'')
		return (1);
	return (0);
}

char	*skip_stars(char *pattern)
{
	while (*pattern == '*')
		pattern++;
	return (pattern);
}
