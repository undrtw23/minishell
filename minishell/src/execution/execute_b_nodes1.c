/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_b_nodes1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 22:56:54 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 02:46:06 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_globbing(char ***args, int quo[3], t_cmd_set *p)
{
	int	i;

	i = -1;
	while ((*args)[++i])
		*args = glob_expander(args, &i, quo, p);
}

int	exec_cmd_node(t_node *node, t_cmd_set *p)
{
	int		status;
	int		quo[3];
	char	*last_arg;

	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	node->cmd->line = remove_dollar_quote(node->cmd->line);
	node->cmd->line = var_expander(node->cmd->line, quo, p);
	node->cmd->args = split_and_ignore_space_if_in_quote(node->cmd->line, " \t");
	handle_globbing(&node->cmd->args, quo, p);
	if (ft_arr_len(node->cmd->args) == 0)
		return (0);
	last_arg = node->cmd->args[ft_arr_len(node->cmd->args) - 1];
	if (last_arg)
		ft_setenv("_", last_arg, p->envp);
	status = exec_node_par_builtins_b(node, node->cmd, p);
	if (status)
		status = p->status_code;
	if (!p->pipe_flag && !is_builtin(node->cmd))
		status = wait_and_set_status(p);
	return (status);
}


