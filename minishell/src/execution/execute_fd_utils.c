/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:39:26 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/05 18:57:25 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	dup2_and_close(int in_fd, int out_fd)
{
	if (in_fd != 0)
	{
		dup2(in_fd, 0);
		close(in_fd);
	}
	if (out_fd != 1)
	{
		dup2(out_fd, 1);
		close(out_fd);
	}
}

void	restore_std_fds(t_node *node, int orig_in, int orig_out)
{
	if (node->in_fd != 0)
		dup2(orig_in, 0);
	if (node->out_fd != 1)
		dup2(orig_out, 1);
	close(orig_in);
	close(orig_out);
}

void	handle_redirections(t_node *node)
{
	if (node->in_fd != 0)
	{
		dup2(node->in_fd, 0);
		close(node->in_fd);
	}
	if (node->out_fd != 1)
	{
		dup2(node->out_fd, 1);
		close(node->out_fd);
	}
}

void	setup_left_node_fds(t_node *node, int fd)
{
	if (node->in_fd != 0 && parent_is_redir(node, 'i'))
		node->left_node->in_fd = node->in_fd;
	else
		node->left_node->in_fd = fd;
	if (node->out_fd != 1 && node->left_node)
		node->left_node->out_fd = node->out_fd;
}

void	setup_left_node_output_fds(t_node *node, int fd)
{
	if (node->out_fd != 1 && parent_is_redir(node, 'o'))
		node->left_node->out_fd = node->out_fd;
	else
		node->left_node->out_fd = fd;
	if (node->in_fd != 0)
		node->left_node->in_fd = node->in_fd;
}
