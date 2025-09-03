/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:59:14 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 21:19:18 by ngaurama         ###   ########.fr       */
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

int	exec_node(t_node *node, t_cmd_set *p)
{
	if (!node)
		return (-1);
	if (node->type == N_CMD)
		return (exec_cmd_node(node, p));
	else if (node->type == N_OR || node->type == N_AND)
		return (exec_node_and_or(node, p));
	else if (node->type == N_PIPE)
		return (exec_pipe_node(node, p));
	else if (node->type == N_READ || node->type == N_READWRITE
		|| node->type == N_HERESTR || node->type == N_HEREDOC
		|| node->type == N_WRITE || node->type == N_APPEND)
		return (exec_redirect_node(node, p));
	else if (node->type == N_SUBSHELL)
		return (exec_subshell_node(node, p));
	else
		return (-1);
}
