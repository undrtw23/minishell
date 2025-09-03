/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node_checks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:42:36 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:46:26 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	check_subshell_brackets(char **args, int len)
{
	int	i;
	int	sh_head[2];

	i = -1;
	while (++i < len && ft_strncmp(args[i], "(", 1))
		;
	sh_head[0] = i;
	i = len;
	while (--i >= 0 && ft_strncmp(args[i], ")", 1))
		;
	sh_head[1] = i;
	return (sh_head[1] - sh_head[0]);
}

int	check_subshell_errors(char **args, int len, t_cmd_set *p)
{
	int	sh_head[2];
	int	i;

	i = -1;
	while (++i < len && ft_strncmp(args[i], "(", 1))
		;
	sh_head[0] = i;
	i = len;
	while (--i >= 0 && ft_strncmp(args[i], ")", 1))
		;
	sh_head[1] = i;
	if (sh_head[0] > 0 && get_precedence(args[sh_head[0] - 1]) < 2)
		return (put_err("Unexpected_Token1", args[sh_head[0] + 1], 2, p), 2);
	else if (sh_head[1] - sh_head[0] == 1)
		return (put_err("Unexpected_Token2", args[sh_head[1]], 2, p), 2);
	else if (sh_head[0] == 0 && (sh_head[1] + 1 < len)
		&& get_precedence(args[sh_head[1] + 1]) < 2)
		return (put_err("Unexpected_Token3", args[sh_head[1] + 1], 2, p), 2);
	return (0);
}

int	check_subshell(t_node *node, t_cmd_set *p, char **args)
{
	int	len;

	(void)node;
	len = ft_arr_len(args);
	return (check_subshell_errors(args, len, p));
}

int	check_redirect_errors(char **args, int i, t_node *nodehead, t_cmd_set *p)
{
	if (!args[i + 1] || (get_nodetype(args[i + 1]) != N_CMD))
	{
		put_err("Unexpected_Token", p->tokens[nodehead->op_token_index + 1], 2,
			p);
		return (1);
	}
	if (p->tokens[nodehead->op_token_index + 2]
		&& !ft_strncmp(p->tokens[nodehead->op_token_index + 2], "(", 1))
	{
		put_err("Unexpected_Token", p->tokens[nodehead->op_token_index + 2], 2,
			p);
		return (1);
	}
	return (0);
}

int	check_redirect_node(char **args, int i, t_node *nodehead, t_cmd_set *p)
{
	(void)p;
	if (check_redirect_errors(args, i, nodehead, p))
		return (1);
	nodehead->file = ft_strdup(args[i + 1]);
	return (0);
}
