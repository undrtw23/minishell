/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node_handlers.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:42:44 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:47:31 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_node	*handle_redirect_node(char **args, int i, t_cmd_set *p, int index_base)
{
	t_node	*nodehead;

	nodehead = malloc_and_init_node(NULL);
	if (!nodehead)
		return (NULL);
	nodehead->type = get_nodetype(args[i]);
	nodehead->op_token_index = i + index_base;
	if (check_redirect_node(args, i, nodehead, p))
	{
		free_node(nodehead);
		return (NULL);
	}
	if (i != 0)
		nodehead->left_node = create_node(trim_array(args, 0, i - 1), p,
				index_base);
	else
		nodehead->left_node = create_node(trim_array(args, i + 2,
					ft_arr_len(args) - 1), p, index_base);
	if (nodehead->left_node)
		nodehead->left_node->parent_node = nodehead;
	return (nodehead);
}

t_node	*handle_subshell_node(char **args, int i, t_cmd_set *p, int index_base)
{
	t_node	*nodehead;

	nodehead = malloc_and_init_node(NULL);
	if (!nodehead)
		return (NULL);
	nodehead->type = get_nodetype(args[i]);
	nodehead->op_token_index = i + index_base;
	if (check_subshell(nodehead, p, args))
	{
		free_node(nodehead);
		return (NULL);
	}
	nodehead->left_node = create_node(trim_array(args, 1, i - 1), p, 1
			+ index_base);
	if (!nodehead->left_node)
	{
		free_node(nodehead);
		return (NULL);
	}
	nodehead->left_node->parent_node = nodehead;
	return (nodehead);
}

t_node	*handle_binary_node(char **args, int i, t_cmd_set *p, int index_base)
{
	t_node	*nodehead;

	nodehead = malloc_and_init_node(NULL);
	if (!nodehead)
		return (NULL);
	nodehead->type = get_nodetype(args[i]);
	nodehead->op_token_index = i + index_base;
	nodehead->left_node = create_node(trim_array(args, 0, i - 1), p,
			index_base);
	if (!nodehead->left_node)
	{
		free_node(nodehead);
		return (NULL);
	}
	nodehead->left_node->parent_node = nodehead;
	nodehead->right_node = create_node(trim_array(args, i + 1, ft_arr_len(args)
				- 1), p, i + 1 + index_base);
	if (!nodehead->right_node)
	{
		free_node(nodehead);
		return (NULL);
	}
	nodehead->right_node->parent_node = nodehead;
	return (nodehead);
}

t_node	*create_node_helper(char **args, t_cmd_set *p, int i, int index_base)
{
	t_node	*nodehead;

	nodehead = NULL;
	if (get_nodetype(args[i]) == N_CMD)
	{
		nodehead = malloc_and_init_node(nodehead);
		if (!nodehead)
			return (NULL);
		nodehead->type = N_CMD;
		nodehead->cmd = parse_cmd(args[i], p);
		return (nodehead);
	}
	else if (get_precedence(args[i]) == 4)
		nodehead = handle_redirect_node(args, i, p, index_base);
	else if (get_nodetype(args[i]) == N_SUBSHELL)
		nodehead = handle_subshell_node(args, i, p, index_base);
	else
		nodehead = handle_binary_node(args, i, p, index_base);
	return (nodehead);
}

t_node	*create_node(char **args, t_cmd_set *p, int index_base)
{
	int		i;
	t_node	*nodehead;

	if (!args || !*args || !*args[0])
		return (NULL);
	nodehead = NULL;
	i = find_lowest_precedence_op(args, p);
	if (i == -1)
		return (free_array(&args), NULL);
	nodehead = create_node_helper(args, p, i, index_base);
	free_array(&args);
	return (nodehead);
}
