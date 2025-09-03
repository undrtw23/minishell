/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_subshell.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:39 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:18:48 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	setup_left_node_readwrite_fds(t_node *node, int fd)
{
	if (node->in_fd != 0 && parent_is_redir(node, 'i'))
		node->left_node->in_fd = node->in_fd;
	else
		node->left_node->in_fd = fd;
	if (node->out_fd != 1)
		node->left_node->out_fd = node->out_fd;
}

void	handle_child_process(t_node *node, t_cmd_set *p)
{
	init_subshell(p, node);
	handle_redirections(node);
	exec_node(node->left_node, p);
	close_fds_except_std();
	free_exit(p, p->status_code, NULL);
}

int	exec_subshell_node(t_node *node, t_cmd_set *p)
{
	pid_t	pid;
	int		status;

	status = 0;
	pid = fork();
	if (pid == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p), 1);
	if (pid > 0)
		handle_parent_process(pid, node, p);
	else if (!pid)
		handle_child_process(node, p);
	if (!p->pipe_flag)
		status = wait_and_set_status(p);
	return (status);
}
