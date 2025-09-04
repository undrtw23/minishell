/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fork_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:41:06 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/04 22:43:18 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_node_par_builtins_b_helper_two(t_node *node, t_cmd *cmd, t_cmd_set *p,
		pid_t pid)
{
	char	*path;
	int		s;

	s = 0;
	if (!pid)
	{
		signal(SIGINT, signals_child);
		signal(SIGQUIT, signals_child);
		dup2_and_close(node->in_fd, node->out_fd);
		close_fds_except_std();
		if (is_builtin(node->cmd))
		{
			s = exec_builtins(node, cmd, p);
			free_exit(p, p->status_code, NULL);
		}
		path = ft_getenv("PATH", p->envp);
		find_cmd_path(p, node, path);
		if (!cmd->cmd_path)
			cmd->cmd_path = ft_strdup(cmd->args[0]);
		free(path);
		execve(cmd->cmd_path, cmd->args, p->envp);
		free_exit(p, p->status_code, NULL);
	}
	return (s);
}

int	exec_node_par_builtins_b_helper_one(pid_t pid, t_cmd_set *p)
{
	if (pid == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p), 1);
	if (pid > 0)
	{
		p->pid_arr[p->pid_index] = pid;
		p->pid_index++;
		p->pid_of_lst_cmd = pid;
	}
	return (0);
}

void	init_orig_fd(int *orig_fd_io)
{
	orig_fd_io[0] = dup(0);
	orig_fd_io[1] = dup(1);
}

int	exec_node_par_builtins_b(t_node *node, t_cmd *cmd, t_cmd_set *p)
{
	int		s;
	int		orig_fd_io[2];
	pid_t	pid;

	s = 0;
	init_orig_fd(orig_fd_io);
	if (p->pipe_flag == 0 && is_builtin(node->cmd))
	{
		if (node->in_fd != 0)
			dup2(node->in_fd, 0);
		if (node->out_fd != 1)
			dup2(node->out_fd, 1);
		s = exec_builtins(node, node->cmd, p);
	}
	else
	{
		pid = fork();
		if ((pid == -1 || pid > 0) && (exec_node_par_builtins_b_helper_one(pid,
					p)))
			return (restore_std_fds(node, orig_fd_io[0], orig_fd_io[1]), 1);
		else
			s = exec_node_par_builtins_b_helper_two(node, cmd, p, pid);
	}
	restore_std_fds(node, orig_fd_io[0], orig_fd_io[1]);
	return (s);
}
