/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_condition_check.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:14:36 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:14:43 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_triple_less_than(char *s, int pos, int quo[3])
{
	return (!quo[2] && s[pos + 1] && s[pos + 2] && s[pos] == '<' && s[pos
			+ 1] == '<' && s[pos + 2] == '<');
}

int	is_double_less_than(char *s, int pos, int quo[3])
{
	return (!quo[2] && s[pos + 1] && s[pos + 2] && s[pos] == '<' && s[pos
			+ 1] == '<' && s[pos + 2] != '<');
}

void	handle_double_less_than(char **s, int i[3], int quo[3], char *tmp[3])
{
	i[1] = i[0] + 2;
	while ((*s)[i[1]] && !quo[2] && (ft_strchr(" \t", (*s)[i[1]])))
		i[1]++;
	i[2] = i[1];
	while ((*s)[i[1]])
	{
		upd_quo(quo, (*s)[i[1]]);
		if (ft_strchr(" \t<>|&()", (*s)[i[1]]) && !quo[2])
			break ;
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
	i[0] += 2;
}
