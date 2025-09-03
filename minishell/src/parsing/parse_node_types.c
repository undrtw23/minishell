/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node_types.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:42:48 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:43:37 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_nodetype	get_nodetype(char *str)
{
	int	l;

	l = ft_strlen(str);
	if (!ft_strncmp("||", str, 2) && l == 2)
		return (N_OR);
	if (!ft_strncmp("&&", str, 2) && l == 2)
		return (N_AND);
	if (!ft_strncmp("|", str, 1) && l == 1)
		return (N_PIPE);
	if (!ft_strncmp("<<", str, 2) && l == 2)
		return (N_HEREDOC);
	if (!ft_strncmp("<", str, 1) && l == 1)
		return (N_READ);
	if (!ft_strncmp(">>", str, 2) && l == 2)
		return (N_APPEND);
	if (!ft_strncmp(">", str, 1) && l == 1)
		return (N_WRITE);
	if (!ft_strncmp("<>", str, 2) && l == 2)
		return (N_READWRITE);
	if (!ft_strncmp("<<<", str, 3) && l == 3)
		return (N_HERESTR);
	if ((!ft_strncmp("(", str, 1) || !ft_strncmp(")", str, 1)) && l == 1)
		return (N_SUBSHELL);
	return (N_CMD);
}

int	get_precedence(char *str)
{
	t_nodetype	type;

	type = get_nodetype(str);
	if (type == N_SUBSHELL)
		return (5);
	if (type == N_WRITE || type == N_APPEND || type == N_READ
		|| type == N_HEREDOC || type == N_READWRITE || type == N_HERESTR)
		return (4);
	if (type == N_PIPE)
		return (3);
	if (type == N_AND || type == N_OR)
		return (2);
	return (0);
}

t_node	*malloc_and_init_node(t_node *node)
{
	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	node->type = N_UNDEFINED;
	node->parent_node = NULL;
	node->left_node = NULL;
	node->right_node = NULL;
	node->file = NULL;
	node->in_fd = 0;
	node->out_fd = 1;
	node->op_token_index = 0;
	node->shlvl = 0;
	return (node);
}
