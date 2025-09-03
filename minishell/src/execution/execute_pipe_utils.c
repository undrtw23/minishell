/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:08 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:16:47 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	setup_pipe_fds(t_node *node)
{
	if (!node->left_node || !node->right_node)
		return (-1);
	if (node->out_fd != 1 && node->right_node
		&& node->right_node->type != N_WRITE
		&& node->right_node->type != N_APPEND)
		node->right_node->out_fd = node->out_fd;
	if (node->in_fd != 0 && node->left_node && node->left_node->type != N_READ
		&& node->left_node->type != N_READWRITE
		&& node->left_node->type != N_HEREDOC
		&& node->left_node->type != N_HERESTR)
		node->left_node->in_fd = node->in_fd;
	return (0);
}

int	create_pipe(int *fd, t_cmd_set *p)
{
	if (pipe(fd) == -1)
	{
		put_err("DupForkPipe_Failed", NULL, 1, p);
		return (1);
	}
	return (0);
}

int	execute_left_pipe(t_node *node, int *fd, t_cmd_set *p)
{
	p->abort = 0;
	node->left_node->out_fd = fd[1];
	exec_node(node->left_node, p);
	close(fd[1]);
	return (0);
}

int	execute_right_pipe(t_node *node, int *fd, t_cmd_set *p)
{
	int	status;

	p->abort = 0;
	node->right_node->in_fd = fd[0];
	status = exec_node(node->right_node, p);
	close(fd[0]);
	return (status);
}

void	handle_pipe_completion(int status, t_cmd_set *p)
{
	p->pipe_flag--;
	if (!p->pipe_flag)
		p->status_code = (wait_and_set_status(p));
	if (!p->status_code && status)
		p->status_code = status;
}
