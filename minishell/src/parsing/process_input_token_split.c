/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_token_split.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:03:48 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:09:09 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**split_by_op_ign_space_in_quote(char *input, char *ops, t_cmd_set *p)
{
	char	**a;
	int		i;

	i = -1;
	a = NULL;
	p->token_count = count_tokens(input, ops);
	if (!p->token_count || p->token_count > 2047)
	{
		if (p->token_count > 2047)
			put_err(NULL, "Token limit surpassed", 1, p);
		return (NULL);
	}
	a = (char **)malloc(sizeof(char *) * (p->token_count + 1));
	if (!a)
		return (put_err(NULL, "Malloc error", 1, p), NULL);
	fill_token_tab(input, &a, ops, p->token_count);
	return (a);
}

int	check_delim_expand(char *lim)
{
	int	i;

	if (!*lim)
		return (-1);
	i = -1;
	while (lim[++i])
	{
		if (lim[i] == '\'' || lim[i] == '\"')
			return (0);
	}
	return (1);
}

int	handle_heredoc_case(t_cmd_set *p, int i, int *line_index)
{
	char	*str[3];

	if (get_nodetype(p->tokens[i + 1]) == N_CMD)
	{
		str[0] = NULL;
		str[1] = NULL;
		str[2] = NULL;
		p->heredoc[i] = read_heredoc_b(str, p->tokens[i + 1], p, line_index);
		if (p->heredoc[i] == -1)
		{
			if (g_exit_status == 130)
				return (p->status_code = 130, 130);
			put_err("NoFile_NoDir", NULL, 1, p);
			return (-1);
		}
	}
	else
	{
		if (g_exit_status == 0)
			g_exit_status = p->status_code;
		return (-2);
	}
	return (0);
}

void	set_final_exit_status(t_cmd_set *p)
{
	if (g_exit_status == 0)
		g_exit_status = p->status_code;
}

int	read_heredocs(t_cmd_set *p, int *line_index, int syntax)
{
	int	i;
	int	result;

	i = -1;
	while (p->tokens[++i] && i < syntax)
	{
		if (get_nodetype(p->tokens[i]) == N_HEREDOC)
		{
			result = handle_heredoc_case(p, i, line_index);
			if (result == 130)
				return (130);
			else if (result == -1)
				break ;
			else if (result == -2)
				return (syntax);
		}
	}
	set_final_exit_status(p);
	return (syntax);
}
