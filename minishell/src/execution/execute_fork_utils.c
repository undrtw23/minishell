/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_fork_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:41:06 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:16:38 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	fork_and_exec_helper(t_node *node, t_cmd *cmd, t_cmd_set *p)
{
	char	*path;

	path = NULL;
	if (node->in_fd == -1 || node->out_fd == -1)
	{
		p->status_code = 1;
		free_exit(p, p->status_code, NULL);
	}
	dup2_and_close(node->in_fd, node->out_fd);
	path = ft_getenv("PATH", p->envp);
	find_cmd_path(p, node, path);
	if (!cmd->cmd_path)
		cmd->cmd_path = ft_strdup(cmd->args[0]);
	free(path);
	ft_printf_fd(2, "sstatuscode=%i\n", p->status_code);
	execve(cmd->cmd_path, cmd->args, p->envp);
	ft_printf_fd(2, "sstatuscode=%i\n", p->status_code);
	free_exit(p, p->status_code, NULL);
	exit(p->status_code);
}

int	fork_and_exec(t_node *node, t_cmd *cmd, t_cmd_set *p)
{
	pid_t	pid;
	char	**s;

	s = NULL;
	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	pid = fork();
	if (pid == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p), 1);
	if (pid > 0)
	{
		p->pid_arr[p->pid_index] = pid;
		p->pid_index++;
		p->pid_of_lst_cmd = pid;
	}
	else if (!pid)
		fork_and_exec_helper(node, cmd, p);
	return (0);
}

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
		close_fds_except_std();
		free_exit(p, p->status_code, NULL);
		exit(p->status_code);
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

int	exec_node_par_builtins_b(t_node *node, t_cmd *cmd, t_cmd_set *p)
{
	int		s;
	int		orig_in;
	int		orig_out;
	pid_t	pid;

	s = 0;
	orig_in = dup(0);
	orig_out = dup(1);
	if (node->in_fd != 0)
		dup2(node->in_fd, 0);
	if (node->out_fd != 1)
		dup2(node->out_fd, 1);
	if (p->pipe_flag == 0 && is_builtin(node->cmd))
		s = exec_builtins(node, node->cmd, p);
	else
	{
		pid = fork();
		if ((pid == -1 || pid > 0) && (exec_node_par_builtins_b_helper_one(pid,
					p)))
			return (restore_std_fds(orig_in, orig_out), 1);
		else
			s = exec_node_par_builtins_b_helper_two(node, cmd, p, pid);
	}
	restore_std_fds(orig_in, orig_out);
	return (s);
}
