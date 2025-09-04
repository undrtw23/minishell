/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect_utils.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:33 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/04 22:33:21 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_open_error(t_node *node, t_cmd_set *p)
{
	if (errno == ENOENT)
		return (put_err("NoFile_NoDir", node->file, 1, p), 1);
	else if (errno == EACCES)
		return (put_err("Perm_Denied", node->file, 1, p), 1);
	else if (errno == EISDIR)
		return (put_err("Is_Directory", node->file, 1, p), 1);
	else if (errno == ENOTDIR)
		return (put_err("Is_Not_Directory", node->file, 1, p), 1);
	else if (errno == ELOOP)
		return (put_err("Loop_Links", node->file, 1, p), 1);
	else
		return (0);
}

int	handle_read_errors(t_cmd_set *p, int err)
{
	if (err)
	{
		p->abort = 1;
		if (!p->pipe_flag)
			p->status_code = 1;
	}
	return (err);
}

int	handle_write_errors(t_cmd_set *p, int err)
{
	if (err)
		p->abort = 1;
	return (err);
}

int	handle_readwrite_errors(t_cmd_set *p, int err)
{
	if (err)
		p->abort = 1;
	return (err);
}

int	check_fd(int fd, t_node *node, t_cmd_set *p)
{
	if (fd == -1)
		return (put_err("bad fd", node->file, 1, p), 1);
	return (0);
}
