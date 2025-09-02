/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_fds.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwang <cwang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:29:05 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/29 14:08:45 by cwang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Main fd helper :
Uses flags to return the correct fd in function of the token found.
Use	access(void) to check if the file to open have the right permissions
Central error messaging.*/
int	get_fd(int oldfd, char *path, int flags[2], t_cmd_set *p)
{
	int	fd;

	if (oldfd > 2)
		close(oldfd);
	if (!path)
		return (-1);
	if (!flags[0] && access(path, F_OK) == -1)
		return (put_err("NoFile_NoDir", path, 1, p), -1);
	if (!flags[0] && access(path, R_OK) == -1)
		return (put_err("Perm_Denied", path, 1, p), -1);
	if (flags[0] && access(path, F_OK) == 0 && access(path, W_OK) == -1)
		return (put_err("Perm_Denied", path, 1, p), -1);
	if (flags[0] && flags[1])
		fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0664);
	else if (flags[0])
		fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0664);
	else
		fd = open(path, O_RDONLY);
	if (fd == -1)
		put_err("Perm_Denied", path, 1, p);
	return (fd);
}

/* Handles '>' (overwrite)
goes to :
else if (flags[0])
	fd = open(path, O_CREAT | O_WRONLY | O_TRUNC, 0664);
Missing file, get error_token_newline
Else error is printed in get_fed and status code is set to 1.
Btw if error *i = -1 trigger :
if (i < 0)
			return (free_tmp_lst(cmds[0], args, temp[1]));
in parse_cmd to stop parsing.*/
t_cmd	*out_fd_truncate(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		node->out_fd = get_fd(node->out_fd, args[*i], flags, p);
	if (!args[*i] || node->out_fd == -1)
	{
		*i = -1;
		if (node->out_fd != -1)
		{
			error_token_newline();
			p->status_code = 2;
		}
		else
			p->status_code = 1;
	}
	return (node);
}

/* Handles '>>' (append)
goes to :
if (flags[0] && flags[1])
	fd = open(path, O_CREAT | O_WRONLY | O_APPEND, 0664);
note that we skip second '>' : args[++(*i)
Same error handling as before.
*/
t_cmd	*out_fd_append(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	int	flags[2];

	flags[0] = 1;
	flags[1] = 1;
	(*i)++;
	if (args[++(*i)])
		node->out_fd = get_fd(node->out_fd, args[*i], flags, p);
	if (!args[*i] || node->out_fd == -1)
	{
		*i = -1;
		if (node->out_fd != -1)
		{
			error_token_newline();
			p->status_code = 2;
		}
		else
			p->status_code = 1;
	}
	return (node);
}

/* handles '<' (input redirection) 
goes to :
else
    fd = open(path, O_RDONLY);
Exact same error handling as before, if this fail, execution continue
( check for the next pipe).*/
t_cmd	*in_fd_read(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	int	flags[2];

	flags[0] = 0;
	flags[1] = 0;
	(*i)++;
	if (args[*i])
		node->in_fd = get_fd(node->in_fd, args[*i], flags, p);
	if (!args[*i] || node->in_fd == -1)
	{
		if (!args[*i])
		{
			error_token_newline();
			p->status_code = 2;
		}
		else
			p->status_code = 1;
		while (args[*i] && args[*i][0] != '|')
			(*i)++;
		(*i)--;
	}
	return (node);
}
