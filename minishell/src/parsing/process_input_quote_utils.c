/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_quote_utils.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:03:27 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:04:42 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_unclosed_quotes(const char *s)
{
	int	squote;
	int	dquote;
	int	i;

	squote = 0;
	dquote = 0;
	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' && dquote == 0)
			squote = !squote;
		else if (s[i] == '\"' && squote == 0)
			dquote = !dquote;
		i++;
	}
	return (squote || dquote);
}

int	handle_uquote(char **s, t_cmd_set *p)
{
	(void)s;
	put_err("Unclosed_Quote", NULL, 2, p);
	return (1);
}

int	is_in_quote(char c, int *q)
{
	q[0] = (q[0] + (!q[1] && c == '\'')) % 2;
	q[1] = (q[1] + (!q[0] && c == '\"')) % 2;
	if (q[0] || q[1])
		return (1);
	else
		return (0);
}

int	is_operator_char(char c, char *ops)
{
	return (ft_strchr(ops, c) != NULL);
}

int	is_whitespace_in_quote(char c, int *q)
{
	return ((c == ' ' || c == '\t') && !q[0] && !q[1]);
}
