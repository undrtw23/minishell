/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_syntax_check.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:03:31 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:09:51 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_first_token(char **tokens, int token_counter, t_cmd_set *p)
{
	if (token_counter > 0 && get_nodetype(tokens[0]) == N_PIPE)
	{
		put_err("Unexpected_Token", tokens[0], 2, p);
		return (0);
	}
	return (-1);
}

int	check_shlvl_bracket(char *token, int shlvl, int i, t_cmd_set *p)
{
	if (shlvl < 0)
	{
		put_err("Unexpected_Token", token, 2, p);
		return (i);
	}
	return (-1);
}

int	check_token_pair(char **tokens, int i, t_cmd_set *p)
{
	if (get_precedence(tokens[i]) == 4 && get_nodetype(tokens[i + 1]) != N_CMD)
		return (put_err("Unexpected_Token", tokens[i], 2, p), (i + 1));
	else if (get_nodetype(tokens[i]) == N_CMD && *tokens[i + 1] == '(')
		return (put_err("Unexpected_Token", tokens[i], 2, p), (i + 1));
	else if (get_nodetype(tokens[i]) == N_SUBSHELL && get_nodetype(tokens[i
				+ 1]) == N_SUBSHELL && *tokens[i] != *tokens[i + 1])
		return (put_err("Unexpected_Token", tokens[i + 1], 2, p), (i + 1));
	else if (*tokens[i] == '(' && get_precedence(tokens[i + 1]) < 4
		&& get_nodetype(tokens[i + 1]) != N_CMD)
		return (put_err("Unexpected_Token", tokens[i + 1], 2, p), (i + 1));
	else if (*tokens[i] == ')' && get_nodetype(tokens[i + 1]) == N_CMD)
		return (put_err("Unexpected_Token", tokens[i + 1], 2, p), (i + 1));
	else if (get_nodetype(tokens[i]) != N_CMD && get_precedence(tokens[i]) < 4
		&& get_nodetype(tokens[i + 1]) != N_CMD && get_precedence(tokens[i
				+ 1]) < 4)
		return (put_err("Unexpected_Token", tokens[i], 2, p), (i + 1));
	else if (get_precedence(tokens[i]) == 4 && get_precedence(tokens[i
				+ 1]) == 4)
		return (put_err("Unexpected_Token", tokens[i], 2, p), (i + 1));
	return (-1);
}

int	check_final_token(char **tokens, int i, int shlvl, t_cmd_set *p)
{
	shlvl = shlvl + (*tokens[i] == '(') - (*tokens[i] == ')');
	if (shlvl != 0 || (get_nodetype(tokens[i]) != N_CMD && *tokens[i] != ')'))
	{
		put_err("Unexpected_Token", tokens[i], 2, p);
		return (i);
	}
	return (-1);
}

int	check_syntax(char **tokens, int token_counter, t_cmd_set *p)
{
	int	i;
	int	shlvl;
	int	result;

	shlvl = 0;
	i = -1;
	result = check_first_token(tokens, token_counter, p);
	if (result != -1)
		return (result);
	while (++i < token_counter - 1)
	{
		shlvl = shlvl + (*tokens[i] == '(') - (*tokens[i] == ')');
		result = check_shlvl_bracket(tokens[i], shlvl, i, p);
		if (result != -1)
			return (result);
		result = check_token_pair(tokens, i, p);
		if (result != -1)
			return (result);
	}
	result = check_final_token(tokens, i, shlvl, p);
	if (result != -1)
		return (result);
	return (999);
}
