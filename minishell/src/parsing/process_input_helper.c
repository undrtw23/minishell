/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_helper.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 21:50:23 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:14:21 by ngaurama         ###   ########.fr       */
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

void	handle_curly_brackets(char **input)
{
	int		i[3];
	int		quo[3];
	char	*tmp[3];

	i[0] = -1;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	if (ft_strnstr(*input, "${", ft_strlen(*input)))
		remove_curly_brackets(input, i, quo, tmp);
}

void	handle_input(char **input, t_cmd_set *p)
{
	int		i[3];
	int		quo[3];
	char	*tmp[3];
	char	*s;

	(void)p;
	init_vars(quo, i, &s, *input);
	while (s[i[0]])
	{
		upd_quo(quo, s[i[0]]);
		if (is_triple_less_than(s, i[0], quo))
			i[0] += 3;
		else if (is_double_less_than(s, i[0], quo))
			handle_double_less_than(&s, i, quo, tmp);
		else
			i[0]++;
	}
	*input = s;
	handle_curly_brackets(input);
}
