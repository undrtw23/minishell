/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_readwrite.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:29 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:17:49 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	should_abort_readwrite_execution(t_node *node, t_cmd_set *p)
{
	if (p->abort && (node->left_node->type == N_CMD
			|| node->left_node->type == N_SUBSHELL))
		return (1);
	return (0);
}

int	execute_left_readwrite_node(t_node *node, t_cmd_set *p, int fd)
{
	int	status;

	status = 0;
	if (!p->abort)
		setup_left_node_readwrite_fds(node, fd);
	if (should_abort_readwrite_execution(node, p))
		return (1);
	status = exec_node(node->left_node, p);
	return (status);
}

int	exec_readwrite_node(t_node *node, t_cmd_set *p)
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
		fd = open_readwrite_file(node, p);
		err = errno;
	}
	err = handle_readwrite_errors(p, err);
	if (node->left_node)
		status = execute_left_readwrite_node(node, p, fd);
	if (fd != -1 && fd != 0)
		close(fd);
	return (err || status);
}

char	*create_herestr_filename(t_node *node, t_cmd_set *p)
{
	char	*tmpstr[2];
	char	*filename;

	tmpstr[0] = ft_itoa(node->op_token_index);
	if (!tmpstr[0])
	{
		put_err("malloc: Cannot allocate memory", node->file, 1, p);
		return (NULL);
	}
	tmpstr[1] = ft_strjoin(node->file, tmpstr[0]);
	if (!tmpstr[1])
	{
		free(tmpstr[0]);
		put_err("malloc: Cannot allocate memory", node->file, 1, p);
		return (NULL);
	}
	filename = ft_strjoin("/tmp/", tmpstr[1]);
	free_all(tmpstr[0], tmpstr[1], NULL, NULL);
	return (filename);
}

int	create_and_write_herestr_file(char *filename, t_node *node, t_cmd_set *p)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0664);
	if (fd == -1)
	{
		put_err("Bad fd", filename, 1, p);
		return (-1);
	}
	write(fd, (const void *)node->file, ft_strlen(node->file));
	write(fd, "\n", 1);
	if (close(fd) == -1)
	{
		put_err("close failed", filename, 1, p);
		return (-1);
	}
	return (0);
}
