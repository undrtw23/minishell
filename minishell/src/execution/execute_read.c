/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:26 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:17:26 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	open_read_file(t_node *node, t_cmd_set *p)
{
	int	fd;
	int	err;

	err = 0;
	errno = 0;
	fd = open(node->file, O_RDONLY);
	if (fd == -1)
	{
		handle_open_error(node, p);
		err = errno;
	}
	return (fd);
}

int	execute_left_read_node(t_node *node, t_cmd_set *p, int fd)
{
	int	status;

	status = 0;
	if (!p->abort)
		setup_left_node_fds(node, fd);
	if (p->abort && (node->left_node->type == N_CMD
			|| node->left_node->type == N_SUBSHELL))
		return (1);
	status = exec_node(node->left_node, p);
	return (status);
}

int	exec_read_node(t_node *node, t_cmd_set *p)
{
	int	status;
	int	fd;
	int	err;

	err = 0;
	fd = 0;
	status = 0;
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	if (!status)
	{
		fd = open_read_file(node, p);
		err = errno;
	}
	err = handle_read_errors(p, err);
	if (node->left_node)
		status = execute_left_read_node(node, p, fd);
	if (fd != -1 && fd != 0)
		close(fd);
	return (err || status);
}

int	handle_heredoc_left_node(t_node *node, t_cmd_set *p, int fd)
{
	int	status;

	status = 0;
	if (!p->abort)
	{
		if (node->in_fd != 0 && parent_is_redir(node, 'i'))
			node->left_node->in_fd = node->in_fd;
		else
			node->left_node->in_fd = fd;
		if (node->out_fd != 1)
			node->left_node->out_fd = node->out_fd;
	}
	if (p->abort && (node->left_node->type == N_CMD
			|| node->left_node->type == N_SUBSHELL))
		return (p->abort);
	status = exec_node(node->left_node, p);
	return (status);
}

int	exec_heredoc_node(t_node *node, t_cmd_set *p)
{
	int	fd;
	int	status;

	status = 0;
	fd = -1;
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	if (!status)
		fd = p->heredoc[node->op_token_index];
	if (node->left_node)
		status = handle_heredoc_left_node(node, p, fd);
	if (fd != -1)
		close(fd);
	return (status);
}
