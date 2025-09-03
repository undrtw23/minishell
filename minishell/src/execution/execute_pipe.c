/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 20:54:08 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_pipe_node(t_node *node, t_cmd_set *p)
{
	int	fd[2];
	int	status;
	int	pipe_result;

	status = 0;
	if (setup_pipe_fds(node) == -1)
		return (-1);
	p->pipe_flag++;
	pipe_result = create_pipe(fd, p);
	if (pipe_result)
		return (pipe_result);
	if (node->left_node)
		execute_left_pipe(node, fd, p);
	if (node->right_node)
		status = execute_right_pipe(node, fd, p);
	handle_pipe_completion(status, p);
	return (status);
}

int	exec_node_and_or(t_node *node, t_cmd_set *p)
{
	int	status;

	if (node->in_fd != 0)
	{
		if (node->left_node)
			node->left_node->in_fd = node->in_fd;
		if (node->right_node)
			node->right_node->in_fd = node->in_fd;
	}
	if (node->out_fd != 1)
	{
		if (node->left_node)
			node->left_node->out_fd = node->out_fd;
		if (node->right_node)
			node->right_node->out_fd = node->out_fd;
	}
	p->abort = 0;
	status = exec_node(node->left_node, p);
	if ((p->status_code == 0 && node->type == N_AND) || (p->status_code != 0
			&& node->type == N_OR))
	{
		p->abort = 0;
		status = exec_node(node->right_node, p);
	}
	return (status);
}

int	parent_is_redir(t_node *node, char c)
{
	t_node		*parent;
	t_nodetype	type;

	parent = node->parent_node;
	if (!parent)
		return (0);
	type = node->parent_node->type;
	return (parent_is_redir_helper(c, type, parent));
}

int	parent_is_redir_helper(char c, t_nodetype type, t_node *parent)
{
	if (c == 'i')
	{
		if (type == N_READ || type == N_READWRITE || type == N_HEREDOC
			|| type == N_HERESTR)
			return (1);
		else if (type == N_WRITE || type == N_APPEND)
			return (parent_is_redir(parent, c));
		else
			return (0);
	}
	if (c == 'o')
	{
		if (type == N_WRITE || type == N_APPEND)
			return (1);
		else if (type == N_READ || type == N_READWRITE || type == N_HEREDOC
			|| type == N_HERESTR)
			return (parent_is_redir(parent, c));
		else
			return (0);
	}
	return (0);
}
