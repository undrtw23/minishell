/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtins.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:37:41 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 20:39:16 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	exec_builtins(t_node *node, t_cmd *cmd, t_cmd_set *p)
{
	char	*a;
	int		n;
	int		s;

	(void)node;
	s = 0;
	n = 0;
	a = cmd->args[0];
	n = ft_strlen(a);
	if (a && n == 4 && !ft_strncmp(a, "echo", 4))
		builtin_echo_b(cmd);
	else if (a && n == 4 && !ft_strncmp(a, "exit", 4))
		s = builtin_exit_b(cmd, p);
	else if (a && n == 2 && !ft_strncmp(a, "cd", 2))
		s = builtin_cd_b(p, cmd->args);
	else if (a && n == 6 && !ft_strncmp(a, "export", 6))
		s = builtin_export_b(p, cmd->args);
	else if (a && n == 5 && !ft_strncmp(a, "unset", 5))
		s = builtin_unset_b(p, cmd->args);
	else if (a && n == 3 && !ft_strncmp(a, "env", 3))
		s = builtin_env_b(p->envp);
	else if (a && n == 3 && !ft_strncmp(a, "pwd", 3))
		s = builtin_pwd_b();
	p->status_code = s;
	return (s);
}

void	wait_and_set_status_helper(t_cmd_set *p, int last_status)
{
	if (WIFSIGNALED(last_status))
		signals_child(WTERMSIG(last_status));
	if (WIFEXITED(last_status))
		p->status_code = WEXITSTATUS(last_status);
	else if (WIFSIGNALED(last_status) && last_status != 13)
		p->status_code = 128 + WTERMSIG(last_status);
}

int	wait_and_set_status(t_cmd_set *p)
{
	int	status;
	int	last_status;
	int	i;

	last_status = -1;
	status = -1;
	i = -1;
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (p->pid_index > 0)
	{
		while (++i < p->pid_index)
		{
			waitpid(p->pid_arr[i], &status, 0);
			if (p->pid_arr[i] == p->pid_of_lst_cmd)
				last_status = status;
			p->pid_arr[i] = 0;
		}
		p->pid_index = 0;
	}
	p->pid_of_lst_cmd = 0;
	wait_and_set_status_helper(p, last_status);
	g_exit_status = p->status_code & 255;
	return (g_exit_status);
}
