/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:37:29 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/01 19:09:05 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Handles parent builtin in the case of only one command
and so the enviroment of the whole shell should be modified
ie cd/exit/unset/export.
Else loops through all command and continue the process of
fork/pipe.*/
int	exec_par_builtins(t_cmd_set *p, t_list *cmd, int n)
{
	char	**a;
	int		s;
	int		one;

	s = 0;
	one = ft_lstsize(p->cmds) == 1;
	while (cmd)
	{
		a = ((t_cmd *)cmd->content)->args;
		n = 0;
		if (a)
			n = ft_strlen(*a);
		if (a && n == 4 && !ft_strncmp(*a, "exit", 4) && one)
			builtin_exit(cmd, p);
		else if (a && n == 2 && !ft_strncmp(*a, "cd", 2) && one)
			s = builtin_cd(p, a);
		else if (a && n == 6 && !ft_strncmp(*a, "export", 6) && one)
			s = builtin_export(p, a);
		else if (a && n == 5 && !ft_strncmp(*a, "unset", 5) && one)
			s = builtin_unset(p, a);
		else
			setup_pipe_and_fork_cmd(p, cmd);
		cmd = cmd->next;
	}
	return (s);
}

/* Create both fd (in/out)
SIGINT and SIGQUIT will be handled by signal_child
Call find cmd_path to resolve binnaries path finding :
Fill the cmd with its path and name...
Create the pipe, if it fails put_err
Call 
Close the parent writing end (doesnt need to write)
Close fd[0] only if the current command not connected to the next.
Close all custom in/out fd's created by the redirs, append...*/
void	*setup_pipe_and_fork_cmd(t_cmd_set *p, t_list *cmd)
{
	int	fd[2];

	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	find_cmd_path(p, cmd, NULL);
	if (pipe(fd) == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p));
	if (!chk_perm_call_child(p, cmd, fd))
		return (NULL);
	close(fd[1]);
	if (cmd->next && !((t_cmd *)cmd->next->content)->in_fd)
		((t_cmd *)cmd->next->content)->in_fd = fd[0];
	else
		close(fd[0]);
	if (((t_cmd *)cmd->content)->in_fd > 2)
		close(((t_cmd *)cmd->content)->in_fd);
	if (((t_cmd *)cmd->content)->out_fd > 2)
		close(((t_cmd *)cmd->content)->out_fd);
	return (NULL);
}
/* Handle SIGINT and SIGQUIT via signal_child
If not a builtin, run execve, all verification are done earlier.
If not execute the corresponding builtin instead, store its exit
code in p_>status_code.
reset it if this is not the last cmd.*/

void	exec_child_builtins(t_cmd_set *p, t_cmd *n, int l, t_list *cmd)
{
	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	p->status_code = 0;
	if (!is_builtin(n) && n->args)
		execve(n->cmd_path, n->args, p->envp);
	else if (n->args && !ft_strncmp(*n->args, "pwd", l) && l == 3)
		p->status_code = builtin_pwd();
	else if (n->args && !ft_strncmp(*n->args, "echo", l) && l == 4)
		p->status_code = builtin_echo(cmd);
	else if (n->args && !ft_strncmp(*n->args, "env", l) && l == 3)
		p->status_code = builtin_env(p->envp);
	else if (n->args && !ft_strncmp(*n->args, "exit", l) && l == 4)
		p->status_code = builtin_exit_child(cmd);
	else if (n->args && !ft_strncmp(*n->args, "cd", l) && l == 2)
		p->status_code = builtin_cd(p, n->args);
	else if (n->args && !ft_strncmp(*n->args, "export", l) && l == 6)
		p->status_code = builtin_export(p, n->args);
	else if (n->args && !ft_strncmp(*n->args, "unset", l) && l == 5)
		p->status_code = builtin_unset(p, n->args);
	if (cmd->next)
		p->status_code = 0;
}
