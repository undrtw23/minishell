/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_herestr.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:50 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:17:56 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	open_herestr_file_for_reading(char *filename, t_node *node, t_cmd_set *p)
{
	int	fd;
	int	err;

	err = 0;
	errno = 0;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		handle_open_error(node, p);
		err = errno;
	}
	return (fd);
}

void	setup_left_node_herestr_fds(t_node *node, int fd)
{
	if (node->in_fd != 0 && parent_is_redir(node, 'i'))
		node->left_node->in_fd = node->in_fd;
	if (node->out_fd != 1 && node->left_node)
		node->left_node->out_fd = node->out_fd;
	if (node->in_fd == 0)
		node->left_node->in_fd = fd;
}

int	execute_left_herestr_node(t_node *node, t_cmd_set *p, int fd)
{
	int	status;

	status = 0;
	if (!p->abort)
		setup_left_node_herestr_fds(node, fd);
	if (p->abort && (node->left_node->type == N_CMD
			|| node->left_node->type == N_SUBSHELL))
		return (1);
	status = exec_node(node->left_node, p);
	return (status);
}

int	exec_herestr_node(t_node *node, t_cmd_set *p)
{
	int		fd;
	char	*filename;
	int		status;

	status = 0;
	fd = 0;
	filename = create_herestr_filename(node, p);
	if (!filename)
		return (1);
	if (create_and_write_herestr_file(filename, node, p) == -1)
		return (free(filename), 1);
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	if (!status)
		fd = open_herestr_file_for_reading(filename, node, p);
	if (errno)
		p->abort = 1;
	if (node->left_node)
		status = execute_left_herestr_node(node, p, fd);
	if (fd != -1 && fd != 1)
		close(fd);
	unlink(filename);
	free(filename);
	return (errno || status);
}

int	should_remove_quote(char *file, int i, int q[2])
{
	if (file[i] == '\'' && !q[1])
		return (1);
	if (file[i] == '\"' && !q[0])
		return (1);
	return (0);
}
