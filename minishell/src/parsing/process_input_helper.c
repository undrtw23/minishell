/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:50:23 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/01 21:10:13 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Checks if the current character is "${" and not inside quotes.
Updates quote states. Used in remove_curly_brackets(). */
int	update_quotes_chk_curly_bracket(int *quotes, char ch, int i[3], char **s)
{
	quotes[0] = (quotes[0] + (!quotes[1] && ch == '\'')) % 2;
	quotes[1] = (quotes[1] + (!quotes[0] && ch == '\"')) % 2;
	if (!quotes[0] && !quotes[1] && (*s)[i[0]] && (*s)[i[0]] == '$' && (*s)[i[0]
		+ 1] && (*s)[i[0] + 1] == '{' && (*s)[i[0] + 2])
		return (1);
	else
		return (0);
}

/* Replaces ${VAR} with $VAR for correct expansion later.
Handles content inside quotes and reconstructs the string. */
void	remove_curly_brackets(char **s, int i[3], int quotes[2], char *tmp[3])
{
	while (*s && i[0] < (int)ft_strlen(*s) && (*s)[++i[0]])
	{
		if (update_quotes_chk_curly_bracket(quotes, (*s)[i[0]], i, s))
		{
			i[1] = i[0] + 2;
			i[2] = i[1];
			while ((*s)[i[1]] && (*s)[i[1]] != ' ' && (*s)[i[1]] != '}')
				i[1]++;
			if ((*s)[i[1]] == '}')
				tmp[2] = ft_substr(*s, i[2], i[1] - i[2]);
			else
				tmp[2] = ft_substr(*s, i[2] - 1, i[1] - i[2] + 1);
			tmp[0] = ft_substr(*s, 0, i[0]);
			tmp[1] = ft_strjoin(tmp[0], "$");
			free(tmp[0]);
			tmp[0] = ft_strjoin(tmp[1], tmp[2]);
			free_all(tmp[1], tmp[2], NULL, NULL);
			tmp[1] = ft_strjoin(tmp[0], "");
			free(tmp[0]);
			tmp[0] = ft_strdup(&(*s)[i[1] + 1]);
			free_all(*s, NULL, NULL, NULL);
			*s = ft_strjoin(tmp[1], tmp[0]);
			free_all(tmp[0], tmp[1], NULL, NULL);
		}
	}
}

/* Checks for heredoc operator (<<) outside of quotes.
Returns 1 if a valid heredoc is detected, 0 if syntax error.
Note that it is for safety as syntax error are already catching most
of it.*/
int	update_quotes_chk_heredoc(int *quo, char ch, int **i, char **s)
{
	quo[0] = (quo[0] + (!quo[1] && ch == '\'')) % 2;
	quo[1] = (quo[1] + (!quo[0] && ch == '\"')) % 2;
	if (!quo[0] && !quo[1] && (*s)[(*i)[0]] && (*s)[(*i)[0]] == '<'
		&& (*s)[(*i)[0] + 1] && (*s)[(*i)[0] + 1] == '<' && (*s)[(*i)[0] + 2]
		&& (*s)[(*i)[0] + 2] == '<')
	{
		(*i)[0] += 3;
		return (0);
	}
	else if (!quo[0] && !quo[1] && (*s)[(*i)[0]] && (*s)[(*i)[0]] == '<'
		&& (*s)[(*i)[0] + 1] && (*s)[(*i)[0] + 1] == '<' && (*s)[(*i)[0] + 2])
	{
		(*i)[0] += 2;
		return (1);
	}
	else
		return (0);
}

/* Scans the input string for heredoc operators (<<) outside quotes.
Put the delimiter between single quotes to prevent expansion later.
Modifies the input string in-place using tmp for partial rebuilding. */
void	process_heredoc(char **s, int i[3], int quo[2], char *tmp[3])
{
	while (*s && i[0] < (int)ft_strlen(*s) && (*s)[++i[0]])
	{
		printf("i[0]=%i s[i[0]]=%c\n", i[0], (*s)[i[0]]);
		if (update_quotes_chk_heredoc(quo, (*s)[i[0]], &i, s))
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
		}
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
	quo[2] = quo[0] || quo[1];
}
/* Handles modifications to the input string before parsing.
Processes heredoc delimiters and expands ${VAR} and $VAR.
Updates the input string in-place with all applied changes. */
void	handle_input(char **input, t_cmd_set *p)
{
	int		i[3];
	int		quo[3];
	char	*tmp[3];
	char	*s;

	(void)p;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0; // = quo[0] || quo[1]
	s = *input;
	i[0] = 0;
	while (s[i[0]])
	{
		upd_quo(quo, s[i[0]]);
		if (!quo[2] && s[i[0] + 1] && s[i[0] + 2] && s[i[0]] == '<' && s[i[0]
			+ 1] == '<' && s[i[0] + 2] == '<')
			i[0] += 3;
		else if (!quo[2] && s[i[0] + 1] && s[i[0] + 2] && s[i[0]] == '<'
			&& s[i[0] + 1] == '<' && s[i[0] + 2] != '<')
		{
			i[1] = i[0] + 2;
			// printf("i0 %i i1 %i\n", i[0], i[1]);
			while (s[i[1]] && !quo[2] && (ft_strchr(" \t", s[i[1]])))
				i[1]++;
			i[2] = i[1];
			while (s[i[1]])
			{
				upd_quo(quo, s[i[1]]);
				// ft_printf_fd(2, "handleinput quo=%i, s[%i]=%c\n", quo[2], i[1], s[i[1]]);
				if (ft_strchr(" \t<>|&()", s[i[1]]) && !quo[2])
					break ;
				i[1]++;
			}
			tmp[2] = ft_substr(s, i[2], i[1] - i[2]);
			//printf("tmp2=%s\n", tmp[2]);
			if (!*tmp[2])
			{
				free(tmp[2]);
				tmp[2] = NULL;
			}
			else
			{
				tmp[0] = ft_substr(s, 0, i[0]);
				tmp[1] = ft_strjoin(tmp[0], "<<'");
				free(tmp[0]);
				tmp[0] = ft_strjoin(tmp[1], tmp[2]);
				free_all(tmp[1], tmp[2], NULL, NULL);
				tmp[1] = ft_strjoin(tmp[0], "'");
				free(tmp[0]);
				tmp[0] = ft_strdup(&s[i[1]]);
				free_all(s, NULL, NULL, NULL);
				s = ft_strjoin(tmp[1], tmp[0]);
				free_all(tmp[0], tmp[1], NULL, NULL);
			}
			i[0] += 2;
		}
		else
			i[0]++;
	}
	*input = s;
	i[0] = -1;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	if (ft_strnstr(*input, "${", ft_strlen(*input)))
		remove_curly_brackets(input, i, quo, tmp);
	// quo[0] = 0;
	// quo[1] = 0;
	// quo[2] = 0;
	// *input = var_expander(*input, quo, p); //expanding moved to exec_node
	//	printf("input=%s\n", *input);
}
