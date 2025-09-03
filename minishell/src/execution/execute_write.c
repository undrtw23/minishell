/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_write.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:42 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:17:40 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	open_write_file(t_node *node)
{
	int	fd;

	fd = 1;
	errno = 0;
	if (node->type == N_WRITE)
		fd = open(node->file, O_CREAT | O_WRONLY | O_TRUNC, 0664);
	else if (node->type == N_APPEND)
		fd = open(node->file, O_CREAT | O_WRONLY | O_APPEND, 0664);
	return (fd);
}

int	should_abort_execution(t_node *node, t_cmd_set *p)
{
	if (p->abort && (node->left_node->type == N_CMD
			|| node->left_node->type == N_SUBSHELL))
		return (1);
	return (0);
}

int	execute_left_write_node(t_node *node, t_cmd_set *p, int fd)
{
	int	status;

	status = 0;
	if (!p->abort)
		setup_left_node_output_fds(node, fd);
	if (should_abort_execution(node, p))
		return (p->abort);
	status = exec_node(node->left_node, p);
	return (status);
}

int	exec_write_or_append_node(t_node *node, t_cmd_set *p)
{
	int	status;
	int	err;
	int	fd;

	status = 0;
	err = 0;
	fd = 1;
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	if (!status)
	{
		fd = open_write_file(node);
		err = errno;
		handle_open_error(node, p);
	}
	err = handle_write_errors(p, err);
	if (node->left_node)
		status = execute_left_write_node(node, p, fd);
	if (fd != -1 && fd != 1)
		close(fd);
	return (err || status);
}

int	open_readwrite_file(t_node *node, t_cmd_set *p)
{
	int	fd;
	int	err;

	err = 0;
	errno = 0;
	fd = open(node->file, O_RDWR | O_CREAT, 0664);
	if (fd == -1)
	{
		handle_open_error(node, p);
		err = errno;
	}
	return (fd);
}
