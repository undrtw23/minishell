/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:37:49 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:56:00 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	*parse_nodes(char **args, t_cmd_set *p)
{
	int		status;
	char	**dup_args;

	dup_args = ft_dup_array(args);
	p->nodes = create_node(dup_args, p, 0);
	if (p->nodes == NULL)
		return (p);
	status = exec_node(p->nodes, p);
	free_node(p->nodes);
	p->nodes = NULL;
	g_exit_status = p->status_code;
	return (p);
}
