/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:30:08 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/07/08 16:58:00 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Calls exec_parent_builtin first to handle cd/export/unset/exit
continue through that.
Ignore SIGINT/SIGQUIT in parent until everything is finished.
If only one cmd (cd/export/unset/exit) status code is saved immediatly.
Wait for the last forked command to finish.
Wait for any other child process to finish.
WIFSIGNALED(status) checked how the child terminated after wait.
WTERMSIG(status) Returns the signal number that caused child to terminate.
WIFEXITED(status) return true if child exit normally (using exit for example)
command not found
exited normally
fallback.
in case of broken piping (yes | head -n 1) the status code should be 0
else if interuped by ctrl + c, it should be 128 + 2 = 13.*/
void	exec_cmd_and_wait(t_cmd_set *p, int status, int tmp[2])
{
	tmp[1] = exec_par_builtins(p, p->cmds, 0);
	if (ft_lstsize(p->cmds) == 1)
		p->status_code = tmp[1];
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (p && p->pid_of_lst_cmd != 0)
	{
		waitpid(p->pid_of_lst_cmd, &status, 0);
		p->pid_of_lst_cmd = 0;
	}
	waitpid(-1, NULL, 0);
	if (WIFSIGNALED(status))
		signals_child(WTERMSIG(status));
	if (WIFEXITED(status))
	{
		if (WEXITSTATUS(status) == 127 && !tmp[1])
			p->status_code = WEXITSTATUS(status);
		else if (!tmp[1])
			p->status_code = WEXITSTATUS(status);
		else if (status && WEXITSTATUS(status))
			p->status_code = WEXITSTATUS(status);
	}
	else if (WIFSIGNALED(status) && status != 13)
		p->status_code = 128 + WTERMSIG(status);
	p->status_code = p->status_code & 255;
}

/* Piping/redir fd's redirection.
example : echo "hey" > output.txt
echo writes to stdout, duping it to fd = 3 makes it writes
to output instead.
First remap in_fd
Then out_fd
If there is a pipe command, redirect stdout to the pipe’s write end.
Close the write end of pipe.*/
static void	*dup_io_fds(t_list *cmd, int fd[2])
{
	t_cmd	*node;

	node = cmd->content;
	if (node->in_fd != 0)
	{
		if (dup2(node->in_fd, 0) == -1)
			return (put_err("DupForkPipe_Failed", NULL, 1, NULL));
		if (node->in_fd != -1)
			close(node->in_fd);
	}
	if (node->out_fd != 1)
	{
		if (dup2(node->out_fd, 1) == -1)
			return (put_err("DupForkPipe_Failed", NULL, 1, NULL));
		if (node->out_fd != -1)
			close(node->out_fd);
	}
	else if (cmd->next && dup2(fd[1], 1) == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, NULL));
	if (fd[1] != -1)
		close(fd[1]);
	return (NULL);
}

/* Sets up the correct in/out redirections or pipes
Executes the command (builtin or binary)
Cleans up and exits with the proper status.
exec_child_builtins(p, n, l, cmd); execute the actual cmd
Don't forget to free all memory after exiting.*/
void	*exec_in_child(t_cmd_set *p, t_list *cmd, int fd[2])
{
	t_cmd	*n;
	int		l;

	n = cmd->content;
	l = 0;
	if (n->args)
		l = ft_strlen(*n->args);
	dup_io_fds(cmd, fd);
	close(fd[0]);
	close_fds_except_std();
	exec_child_builtins(p, n, l, cmd);
	if (cmd->next != NULL)
		p->status_code = 0;
	l = p->status_code;
	if (p && p->cmds)
		ft_lstclear(&p->cmds, free_lst);
	if (p && p->envp)
		free_array(&p->envp);
	exit(l);
}

/* pid store the result of fork.
if there is no next command and we're in parent process
save the pid in pid_of_lst_cmd.
if pid < 0 forking has failed, close fd's and throw error
Else continue with execution.*/
void	create_fork(t_cmd_set *p, t_list *cmd, int fd[2])
{
	pid_t	pid;

	signal(SIGINT, signals_child);
	signal(SIGQUIT, signals_child);
	pid = fork();
	if (!cmd->next && pid > 0)
		p->pid_of_lst_cmd = pid;
	if (pid < 0)
	{
		if (fd[0] != -1)
			close(fd[0]);
		if (fd[1] != -1)
			close(fd[1]);
		put_err("DupForkPipe_Failed", NULL, 1, p);
	}
	else if (!pid)
		exec_in_child(p, cmd, fd);
}

/* Try open dir to check if its a directory
If in/out fd are invalid, skip execution
if access to the command or is_builtin, continue to forking.
If directory or not executable throw 126.
else throw 127.
Return a non-NULL string to significate succes.*/
void	*chk_perm_call_child(t_cmd_set *p, t_list *cmd, int fd[2])
{
	t_cmd	*n;
	DIR		*dir;

	n = cmd->content;
	dir = NULL;
	if (n->args)
		dir = opendir(*n->args);
	if (n->in_fd == -1 || n->out_fd == -1)
		return (NULL);
	if ((n->cmd_path && access(n->cmd_path, X_OK) == 0) || is_builtin(n))
		create_fork(p, cmd, fd);
	else if (!is_builtin(n) && ((n->cmd_path && !access(n->cmd_path, F_OK))
			|| dir))
		p->status_code = 126;
	else if (!is_builtin(n) && n->args)
		p->status_code = 127;
	if (dir)
		closedir(dir);
	return ("");
}
