/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_handle_helper.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:12:55 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:13:10 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
/* Scans the input string for heredoc operators (<<) outside quotes.
Put the delimiter between single quotes to prevent expansion later.
Modifies the input string in-place using tmp for partial rebuilding. */

void	construct_heredoc_string(char **s, int i[3], char *tmp[3])
{
	tmp[0] = ft_substr(*s, 0, i[0]);
	tmp[1] = ft_strjoin(tmp[0], "<<'");
	free(tmp[0]);
	tmp[0] = ft_strjoin(tmp[1], tmp[2]);
	free_all(tmp[1], tmp[2], NULL, NULL);
	tmp[1] = ft_strjoin(tmp[0], "'");
	free(tmp[0]);
	tmp[0] = ft_strdup(&(*s)[i[1]]);
	free_all(*s, NULL, NULL, NULL);
	*s = ft_strjoin(tmp[1], tmp[0]);
	free_all(tmp[0], tmp[1], NULL, NULL);
}

void	handle_heredoc_expansion(char **s, int i[3], int quo[2], char *tmp[3])
{
	i[1] = i[0] + 2;
	while ((*s)[i[1]] && (ft_strchr(" \t", (*s)[i[1]])))
		i[1]++;
	quo[0] = (quo[0] + (!quo[1] && (*s)[i[1]] == '\'')) % 2;
	quo[1] = (quo[1] + (!quo[0] && (*s)[i[1]] == '\"')) % 2;
	i[2] = i[1];
	while ((*s)[i[1]] && (*s)[i[1]] != ' ' && (*s)[i[1]] != '\t'
		&& (!ft_strchr("<>|&()", (*s)[i[1]]) || quo[0] || quo[1]))
	{
		quo[0] = (quo[0] + (!quo[1] && (*s)[i[1]] == '\'')) % 2;
		quo[1] = (quo[1] + (!quo[0] && (*s)[i[1]] == '\"')) % 2;
		i[1]++;
	}
	tmp[2] = ft_substr(*s, i[2], i[1] - i[2]);
	if (!*tmp[2])
	{
		free(tmp[2]);
		tmp[2] = NULL;
	}
	else
		construct_heredoc_string(s, i, tmp);
}

void	process_heredoc(char **s, int i[3], int quo[2], char *tmp[3])
{
	while (*s && i[0] < (int)ft_strlen(*s) && (*s)[++i[0]])
	{
		printf("i[0]=%i s[i[0]]=%c\n", i[0], (*s)[i[0]]);
		if (update_quotes_chk_heredoc(quo, (*s)[i[0]], &i, s))
			handle_heredoc_expansion(s, i, quo, tmp);
	}
}

void	upd_quo(int *quo, int c)
{
	int	qs;
	int	qd;

	qs = quo[0];
	qd = quo[1];
	if (c == '\'' && !qd)
		qs = !qs;
	if (c == '\"' && !qs)
		qd = !qd;
	quo[0] = qs;
	quo[1] = qd;
	quo[2] = (quo[0] || quo[1]);
}

/* Handles modifications to the input string before parsing.
Processes heredoc delimiters and expands ${VAR} and $VAR.
Updates the input string in-place with all applied changes. */

void	init_vars(int quo[3], int i[3], char **s, char *input)
{
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	*s = input;
	i[0] = 0;
}
