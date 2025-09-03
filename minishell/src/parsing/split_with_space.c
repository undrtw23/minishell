/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_with_space.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:40:04 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 20:40:31 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Count the number of token the input string should produce.
i[0] is the index as you scan s
i[1] is the number of tokens found so far.
As usual quotes tracking via q[0] and q[1], with a change :
q[1] hold here 0 : if not in any quote
34 : double quote
39 : single quote
while we are in quotes, count as one token*/
int	token_counter(const char *s, char *c, int i[2])
{
	int	q[2];

	q[0] = 0;
	q[1] = 0;
	while (s[i[0]] != '\0')
	{
		if (!ft_strchr(c, s[i[0]]))
		{
			i[1]++;
			while (s[i[0]] && (!ft_strchr(c, s[i[0]]) || q[0]))
			{
				if (!q[1] && s[i[0]] && (s[i[0]] == '\"' || s[i[0]] == '\''))
					q[1] = s[i[0]];
				q[0] = (q[0] + (s[i[0]] == q[1])) % 2;
				q[1] *= q[0] != 0;
				i[0]++;
			}
			if (q[0])
				return (-1);
		}
		else
			i[0]++;
	}
	return (i[1]);
}

/* Besides classic quotes handling :
first while : skips space before token, i[0] store the position of token
and store it in i[1].
Advance until next space(if not in quotes)
i[2] : token index
Defensive empty token "\0" if we go beyond input.*/
char	**split_with_quotes(char **tmpstr, char *s, char *set, int i[3])
{
	int	s_len;
	int	q[3];

	q[0] = 0;
	q[1] = 0;
	q[2] = 0;
	s_len = ft_strlen(s);
	while (s[i[0]])
	{
		while (s[i[0]] && ft_strchr(set, s[i[0]]) && s[i[0]] != '\0')
			i[0]++;
		i[1] = i[0];
		while (s[i[0]] && (!ft_strchr(set, s[i[0]]) || q[2]))
		{
			upd_quo(q, s[i[0]]);
			i[0]++;
		}
		if (i[1] >= s_len)
			tmpstr[i[2]++] = "\0";
		else
			tmpstr[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
	}
	return (tmpstr);
}

/* Pre-tokenizer
Count token for spliting.
Malloc the right number of *str (+1 for NULL)
Actual spliting.*/
char	**split_and_ignore_space_if_in_quote(char *s, char *set)
{
	char	**tmpstr;
	int		nwords;
	int		i[3];
	int		counts[2];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	counts[0] = 0;
	counts[1] = 0;
	if (!s)
		return (NULL);
	nwords = token_counter(s, set, counts);
	if (nwords == -1)
		return (NULL);
	tmpstr = malloc((nwords + 1) * sizeof(char *));
	if (tmpstr == NULL)
		return (put_err(NULL, "Malloc failed", 1, NULL), NULL);
	tmpstr = split_with_quotes(tmpstr, s, set, i);
	tmpstr[nwords] = NULL;
	return (tmpstr);
}
