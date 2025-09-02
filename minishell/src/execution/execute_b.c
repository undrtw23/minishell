/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_b.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 20:59:14 by alsima            #+#    #+#             */
/*   Updated: 2025/09/02 22:14:23 by alsima           ###   ########.fr       */
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
	close_fds_except_std();
}
int	fork_and_exec(t_node *node, t_cmd *cmd, t_cmd_set *p)
{
	pid_t	pid;
	char	**s;
	char	*path;

	path = NULL;
	s = NULL;
	// ft_printf_fd(1,
	//	"IN forkandexec cmd args[0]=%s path=%s infd=%d outfd=%d\n",
	//	cmd->args[0], cmd->cmd_path, node->in_fd, node->out_fd);
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
		// ft_printf_fd(1, "IN parent\n");
	}
	else if (!pid)
	{
		if (node->in_fd == -1 || node->out_fd == -1)
		{
			p->status_code = 1;
			free_exit(p, p->status_code, NULL);
		}
		dup2_and_close(node->in_fd, node->out_fd);
		// ft_printf_fd(1, "IN child node_in=%i node_out=%i\n", node->in_fd,
		// node->out_fd);
		path = ft_getenv("PATH", p->envp);
		find_cmd_path(p, node, path);
		// ft_printf_fd(2, "N_CMD CHILD\n");
		// print_tab(cmd->args);
		if (!cmd->cmd_path)
			cmd->cmd_path = ft_strdup(cmd->args[0]);
		// ft_printf_fd(2, "path =~%s~\n", cmd->cmd_path);
		free(path);
		ft_printf_fd(2, "sstatuscode=%i\n", p->status_code);
		execve(cmd->cmd_path, cmd->args, p->envp);
		// put_err("No_Cmd", cmd->args[0], 127, p);
		ft_printf_fd(2, "sstatuscode=%i\n", p->status_code);
		free_exit(p, p->status_code, NULL);
		exit(p->status_code);
	}
	return (0);
}

void	restore_std_fds(int orig_in, int orig_out)
{
	if (orig_in != 0)
		dup2(orig_in, 0);
	if (orig_out != 1)
		dup2(orig_out, 1);
	close(orig_in);
	close(orig_out);
}

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
	// ft_printf_fd(2, "execbuiltins s=%i\n", s);
	p->status_code = s;
	return (s);
}

int	exec_node_par_builtins_b(t_node *node, t_cmd *cmd, t_cmd_set *p)
{
	int		s;
	int		orig_in;
	int		orig_out;
	int		restore;
	pid_t	pid;
	char	*path;

	// ft_printf_fd(1,
	//	"IN forkandexec cmd args[0]=%s path=%s infd=%d outfd=%d\n",
	// 	cmd->args[0], cmd->cmd_path, node->in_fd, node->out_fd);
	s = 0;
	restore = 0;
	orig_in = dup(0);
	orig_out = dup(1);
	// if (node->in_fd == -1 || node->out_fd == -1)
	// {
	// 	p->status_code = 1;
	// 	free_exit(p, p->status_code, NULL);
	// }
	if (node->in_fd != 0)
		dup2(node->in_fd, 0);
	if (node->out_fd != 1)
		dup2(node->out_fd, 1);
	if (p->pipe_flag == 0 && is_builtin(node->cmd))
		s = exec_builtins(node, node->cmd, p);
	else
	{
		pid = fork();
		if (pid == -1)
			return (put_err("DupForkPipe_Failed", NULL, 1, p), 1);
		if (pid > 0)
		{
			p->pid_arr[p->pid_index] = pid;
			p->pid_index++;
			p->pid_of_lst_cmd = pid;
			// ft_printf_fd(1, "IN parent\n");
		}
		else if (!pid)
		{
			signal(SIGINT, signals_child);
			signal(SIGQUIT, signals_child);
			if (is_builtin(node->cmd))
			{
				s = exec_builtins(node, cmd, p);
				// ft_printf_fd(2,
				// "exec_nodeparbuiltins in fork builtin return (status=%i\n",
				// s));
				free_exit(p, p->status_code, NULL);
			}
			// dup2_and_close(node->in_fd, node->out_fd);
			// ft_printf_fd(1, "IN child node_in=%i node_out=%i\n", node->in_fd,
			// node->out_fd);
			path = ft_getenv("PATH", p->envp);
			find_cmd_path(p, node, path);
			// ft_printf_fd(2, "N_CMD CHILD\n");
			// print_tab(cmd->args);
			if (!cmd->cmd_path)
				cmd->cmd_path = ft_strdup(cmd->args[0]);
			// ft_printf_fd(2, "path =~%s~\n", cmd->cmd_path);
			// print_tab(cmd->args);
			free(path);
			execve(cmd->cmd_path, cmd->args, p->envp);
			// put_err("No_Cmd", cmd->args[0], 127, p);
			// ft_printf_fd(2, "sstatuscode=%i\n", p->status_code);
			close_fds_except_std();
			free_exit(p, p->status_code, NULL);
			exit(p->status_code);
		}
		// if (node->out_fd != 1)
		// 	close(node->out_fd);
		// s = p->status_code;
	}
	restore_std_fds(orig_in, orig_out);
	return (s);
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
	if (WIFSIGNALED(last_status))
		signals_child(WTERMSIG(last_status));
	if (WIFEXITED(last_status))
	{
		// ft_printf_fd(2, "waitandsets WIFEXITED last_status=%i\n",
		// last_status);
		p->status_code = WEXITSTATUS(last_status);
		// ft_printf_fd(2, "waitandsetS if WIFEXITED p->statuscode=%i\n",
		// p->status_code);
	}
	else if (WIFSIGNALED(last_status) && last_status != 13)
		p->status_code = 128 + WTERMSIG(last_status);
	// ft_printf_fd(2, "waitandset before g_exit=%i pstatus=%i\n",
	// g_exit_status, p->status_code);
	g_exit_status = p->status_code & 255;
	// ft_printf_fd(2, "waitandset after g_exit=%i\n", g_exit_status);
	return (g_exit_status);
}

int	find_glob(char *token, int *quo)
{
	while (*token)
	{
		upd_quo(quo, *token);
		if (!quo[2] && *token == '*')
			return (1);
		token++;
	}
	return (0);
}

char	*advance_str_past_q(char *headglob, int *quo, int prev_q_state)
{
	while (*headglob && (*headglob == '\'' || *headglob == '\"'))
	{
		prev_q_state = quo[2];
		upd_quo(quo, *headglob);
		headglob++;
	}
	return (headglob);
}

int	match_glob(char *glob, char *file)
{
	char	*idxglob;
	char	*idxfile;
	char	*headfile;
	char	*headglob;
	int		quo[3];
	int		skip;
	int		prev_q_state;
	int		headqstate[3];

	prev_q_state = 0;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	idxglob = glob;
	idxfile = file;
	skip = 0;
	// ft_printf_fd(2, "matchglob glob=%s file=%s\n", glob, file);
	while (*idxglob)
	{
		// ft_printf_fd(2, "	matchglob start idxglob=%s idxfile=%s\n",
		//		idxglob,
		// 		idxfile);
		skip = 0;
		while (*idxglob && !quo[2] && *idxglob == '*')
		{
			idxglob++;
			skip = 1;
		}
		if (!*idxglob)
			return (1);
		if (*idxglob && !*idxfile)
			return (0);
		headfile = idxfile;
		headglob = idxglob;
		headqstate[0] = quo[0];
		headqstate[1] = quo[1];
		headqstate[2] = quo[2];
		while (*headglob && *headfile && (*headglob != '*' || quo[2]))
		{
			prev_q_state = quo[2];
			upd_quo(quo, *headglob);
			// ft_printf_fd(2,
			// 	"matchglob mid hglob=%s hfile=%s quo[2]=%i prevqs=%i\n",
			// 	headglob, headfile, quo[2], prev_q_state);
			if (quo[2] != prev_q_state && *(headglob + 1))
			{
				headglob++;
				idxglob++;
				continue ;
			}
			if (*headglob == '*' && !quo[2])
			{
				idxglob = headglob;
				continue ;
			}
			// ft_printf_fd(2,
			// 	"matchglob beforeskip hglob=%s hfile=%s quo[2]=%i prevqs=%i\n",
			// 	headglob, headfile, quo[2], prev_q_state);
			if (skip)
			{
				while (headfile)
				{
					headfile = ft_strchr(headfile, *headglob);
					if (!headfile)
						return (0);
					while (*headfile && *headglob && (*headglob != '*'
							|| quo[2]) && (*headfile == *headglob))
					{
						// ft_printf_fd(2, "skipin2ndwhile hglob=%s hfile=%s\n",
						// 	headglob, headfile);
						headglob++;
						headfile++;
						if (!*headglob && *headfile && ((headglob
									- idxglob) == 1))
						{
							headglob = idxglob;
							headfile = ft_strchr(headfile, *headglob);
							continue ;
						}
						headglob = advance_str_past_q(headglob, quo,
								prev_q_state);
					}
					// ft_printf_fd(2, "skipafter2ndwhile hglob=%s hfile=%s\n",
					// 	headglob, headfile);
					if (!*headfile || !*headglob || (*headglob == '*'
							&& !quo[2]))
						break ;
					else
					{
						headglob = idxglob;
						headfile++;
						headfile = ft_strchr(headfile, *headglob);
					}
				}
				skip = 0;
			}
			else if (*headfile != *headglob)
				return (0);
			else if (*headfile == *headglob)
			{
				headfile++;
				headglob++;
			}
			headglob = advance_str_past_q(headglob, quo, prev_q_state);
			if (!headfile)
				return (0);
			// ft_printf_fd(2, "	matchglob headglob=%s headfile=%s\n",
			//		headglob,
			// 		headfile);
			if (!*headglob)
			{
				if (!*headfile)
					return (1);
				else
				{
					headglob = idxglob;
					quo[0] = headqstate[0];
					quo[1] = headqstate[1];
					quo[2] = headqstate[2];
					continue ;
				}
			}
			idxfile = headfile;
			idxglob = headglob;
		}
	}
	return (1);
}

char	**expand_glob(char *glob, t_cmd_set *p)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**exp;
	char			*file[2];

	exp = NULL;
	dir = opendir(".");
	if (!dir)
		put_err("opendir error", NULL, 1, p);
	(void)p;
	entry = readdir(dir);
	// ft_printf_fd(2, "expandglob glob=%s\n", glob);
	// remove_token_brackets(glob);
	// ft_printf_fd(2, "expandglob glob=%s\n", glob);
	while (entry != NULL)
	{
		file[0] = NULL;
		file[0] = entry->d_name;
		entry = readdir(dir);
		if (file[0] && file[0][0] == '.')
			continue ;
		// ft_printf_fd(2, "expandglob glob=%s, file=%s\n", glob, file[0]);
		if (match_glob(glob, file[0]))
			exp = ft_array_insert(exp, file[0]);
	}
	closedir(dir);
	return (exp);
}

char	**glob_expander(char ***args, int *i, int quo[3], t_cmd_set *p)
{
	char	**exp;
	int		expand;

	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	(void)p;
	expand = 0;
	exp = NULL;
	// print_tab(*args);
	// ft_printf_fd(2, " before fndglob args[%i]=%s\n", (int)(*i), *(*args
	// + *i));
	expand = find_glob(*(*args + *i), quo);
	// ft_printf_fd(2, "globexpander token=%s expand=%i\n", *(*args + *i),
	// expand);
	if (ft_strcmp("export", *(*args)) && expand)
	{
		exp = expand_glob(*(*args + *i), p);
		// print_tab(exp);
		if (!exp)
		{
			// ft_printf_fd(2, "globexpander glob 0 finds glob=%s\n",
			// *(*args + *i));
			remove_token_brackets(*(*args + *i));
			return (*args);
		}
		*args = ft_array_replace(args, exp, *i);
		*i = *i + ft_arr_len(exp) - 1;
		free_array(&exp);
	}
	else
		remove_token_brackets(*(*args + *i));
	return (*args);
}

int	exec_cmd_node(t_node *node, t_cmd_set *p)
{
	int		status;
	int		quo[3];
	int		i;
	char	*last_arg;

	last_arg = NULL;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	// ft_printf_fd(2, "execcmdnode cmdline=%s\n", node->cmd->line);
	node->cmd->line = remove_dollar_quote(node->cmd->line);
	node->cmd->line = var_expander(node->cmd->line, quo, p);
	node->cmd->args = split_and_ignore_space_if_in_quote(node->cmd->line,
			" \t");
	// print_tab(node->cmd->args);
	i = -1;
	// ft_printf_fd(2, "execcmdnode after split\n");
	while (node->cmd->args[++i])
		node->cmd->args = glob_expander(&node->cmd->args, &i, quo, p);
	if (ft_arr_len(node->cmd->args) == 0)
		return (0);
	last_arg = node->cmd->args[ft_arr_len(node->cmd->args) - 1];
	if (last_arg)
		ft_setenv("_", last_arg, p->envp);
	// node->cmd->line = glob_expander(node->cmd->line, quo, p);
	// remove_token_brackets(node->cmd->line);
	// ft_printf_fd(2, "execcmdnode cmd->line=%s\n", node->cmd->line);
	// ft_printf_fd(2, "execcmdnode after globexpander\n");
	// print_tab(node->cmd->args);
	// i = -1;
	// while (node->cmd->args[++i])
	// 	remove_token_brackets(node->cmd->args[i]);
	// print_tab(node->cmd->args);
	// ft_printf_fd(1, "IN EXEC_CMD+NODE node_in=%i node_out=%i\n",
	// node->in_fd,
	// node->out_fd);
	// ft_printf_fd(1, "IN EXEC_CMD\n");
	status = exec_node_par_builtins_b(node, node->cmd, p);
	if (status)
		status = p->status_code;
	// ft_printf_fd(1, "	after execve\n");
	if (!p->pipe_flag && !is_builtin(node->cmd))
		status = wait_and_set_status(p);
	return (status);
}

int	exec_pipe_node(t_node *node, t_cmd_set *p)
{
	int	fd[2];
	int	status;

	status = 0;
	if (!node->left_node || !node->right_node)
		return (-1);
	p->pipe_flag++;
	if (node->out_fd != 1 && node->right_node
		&& node->right_node->type != N_WRITE
		&& node->right_node->type != N_APPEND)
		node->right_node->out_fd = node->out_fd;
	if (node->in_fd != 0 && node->left_node && node->left_node->type != N_READ
		&& node->left_node->type != N_READWRITE
		&& node->left_node->type != N_HEREDOC
		&& node->left_node->type != N_HERESTR)
		node->left_node->in_fd = node->in_fd;
	if (pipe(fd) == -1)
	{
		put_err("DupForkPipe_Failed", NULL, 1, p);
		return (1);
	}
	if (node->left_node)
	{
		p->abort = 0;
		node->left_node->out_fd = fd[1];
		exec_node(node->left_node, p);
		close(fd[1]);
	}
	//	ft_printf_fd(1, "IN EXEC_PIPE\n");
	if (node->right_node)
	{
		p->abort = 0;
		node->right_node->in_fd = fd[0];
		status = exec_node(node->right_node, p);
		// ft_printf_fd(2, "exec_pipe right->node return status=%i\n",
		// status);
		close(fd[0]);
	}
	p->pipe_flag--;
	// if (node->in_fd != 0)
	// 	close(node->in_fd);
	// if (node->out_fd != 1)
	// 	close(node->out_fd);
	if (!p->pipe_flag)
		p->status_code = (wait_and_set_status(p));
	if (!p->status_code && status)
		p->status_code = status;
	// ft_printf_fd(2, "execpipe p->status=%i g_status=%i status=%i\n",
	// p->status_code, g_exit_status, status);
	return (status);
}

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

int	parent_is_redir(t_node *node, char c)
{
	t_node	*parent;
	t_nodetype	type;
	
	parent = node->parent_node;
	if (!parent)
		return (0);
	type = node->parent_node->type;
	if (c == 'i')
	{
		if (type == N_READ || type == N_READWRITE
		|| type == N_HEREDOC || type == N_HERESTR)
			return (1);
		else if (type == N_WRITE || type == N_APPEND)
			return (parent_is_redir(parent, c));
		else
			return (0);
	}
	if (c == 'o')
	{
		if (type == N_WRITE || type == N_APPEND)
			return (1);
		else if (type == N_READ || type == N_READWRITE
		|| type == N_HEREDOC || type == N_HERESTR)
			return (parent_is_redir(parent, c));
		else
			return (0);
	}
	return (0);
}

int	exec_read_node(t_node *node, t_cmd_set *p)
{
	int	status;
	int	fd;
	int	err;

	err = 0;
	fd = 0;
	status = 0;
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	// ft_printf_fd(2, "IN EXEC_READ nodein=%i nodeout=%i\n", node->in_fd,
	// 	node->out_fd);
	// if (!node->file
	//		&& get_precedence(p->tokens[node->op_token_index]) > 1)
	// 	return (put_err("Unexpected_Token", p->tokens[node->op_token_index],
	//			2,
	// 			p), 2);
	if (!status)
	{
		errno = 0;
		fd = open(node->file, O_RDONLY);
		handle_open_error(node, p);
		err = errno;
	}
	if (err)
	{
		p->abort = 1;
		if (!p->pipe_flag)
			p->status_code = 1;
	}
	// ft_printf_fd(2,
	//"INEXEC_READ nodein=%i nodeout=%i status=%i err=%i\n",
	//	node->in_fd, node->out_fd, status, err);
	// ft_printf_fd(1, "wtf\n");
	//parent_redir = parent_is_redir(node, 'i');
	if (node->left_node)
	{
		if (!p->abort)
		{
			if (node->in_fd != 0 && parent_is_redir(node, 'i'))
				node->left_node->in_fd = node->in_fd;
			else
				node->left_node->in_fd = fd;
			if (node->out_fd != 1 && node->left_node)
				node->left_node->out_fd = node->out_fd;
		}
		if (p->abort && (node->left_node->type == N_CMD
				|| node->left_node->type == N_SUBSHELL))
			return (1);
		status = exec_node(node->left_node, p);
	}
	if (fd != -1 && fd != 0)
		close(fd);
	return (err || status);
}

int	exec_heredoc_node(t_node *node, t_cmd_set *p)
{
	int	fd;
	int	status;

	status = 0;
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	// if (!status)
	// {
	// 	errno = 0;
	// 	fd = open(node->file, O_RDONLY);
	// 	handle_open_error(node, p);
	// 	err = errno;
	// }
	if (!status)
		fd = p->heredoc[node->op_token_index];
	// if (check_fd(fd, node, p))
	// 	return (1);
	if (node->left_node)
	{
		if (!p->abort)
		{
			// ft_printf_fd(2, "execheredocnode parentisredir=%i\n", parent_is_redir(node, 'i'));
			if (node->in_fd != 0 && parent_is_redir(node, 'i'))
				node->left_node->in_fd = node->in_fd;
			else
				node->left_node->in_fd = fd;
			if (node->out_fd != 1)
				node->left_node->out_fd = node->out_fd;
		}
		if (p->abort && (node->left_node->type == N_CMD
				|| node->left_node->type == N_SUBSHELL))
			return (p->abort);
		status = exec_node(node->left_node, p);
	}
	if (fd != -1)
		close(fd);
	return (status);
	// ft_printf_fd(1, "IN EXEC_HEREDOC NODE\n");
	// ft_printf_fd(1, "node->file=%s node->in_fd=%i node->out_fd=%i\n",
	// 	node->file, node->in_fd, node->out_fd);
	// ft_printf_fd(1,
	// 	"IN EXEC_HEREDOC_NODE node->op_token_index=%i p->heredoc=%i\n",
	// 	node->op_token_index, p->heredoc[node->op_token_index]);
}

int	exec_write_or_append_node(t_node *node, t_cmd_set *p)
{
	int	status;
	int	err;
	int	fd;

	status = 0;
	err = 0;
	fd = 1;
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	// ft_printf_fd(2, "in exec writeappend file=%s status=%i fdin=%i\n",
	// 	node->file, status, node->in_fd);
	if (!status)
	{
		errno = 0;
		if (node->type == N_WRITE)
			fd = open(node->file, O_CREAT | O_WRONLY | O_TRUNC, 0664);
		else if (node->type == N_APPEND)
			fd = open(node->file, O_CREAT | O_WRONLY | O_APPEND, 0664);
		err = errno;
		handle_open_error(node, p);
	}
	if (err)
		p->abort = 1;
	if (node->left_node)
	{
		if (!p->abort)
		{
			if (node->out_fd != 1 && parent_is_redir(node, 'o'))
				node->left_node->out_fd = node->out_fd;
			else
				node->left_node->out_fd = fd;
			if (node->in_fd != 0)
				node->left_node->in_fd = node->in_fd;
		}
		if (p->abort && (node->left_node->type == N_CMD
				|| node->left_node->type == N_SUBSHELL))
			return (p->abort);
		status = exec_node(node->left_node, p);
	}
	// ft_printf_fd(2, "in execwriteappend err=%i statusleftnode=%i\n", err,
	//	status);
	if (fd != -1 && fd != 1)
		close(fd);
	// ft_printf_fd(2, "in execwriteappend err=%i\n", err);
	return (err || status);
}

int	check_fd(int fd, t_node *node, t_cmd_set *p)
{
	if (fd == -1)
		return (put_err("bad fd", node->file, 1, p), 1);
	return (0);
}

int	exec_readwrite_node(t_node *node, t_cmd_set *p)
{
	int	status;
	int	fd;
	int	err;

	err = 0;
	fd = 0;
	status = 0;
	// ft_printf_fd(1, "IN EXEC_READWRITE nodein=%i nodeout=%i\n",
	// node->in_fd,
	// 	node->out_fd);
	// if (!node->file
	//	&& get_precedence(p->tokens[node->op_token_index]) > 1)
	// 	return (put_err("Unexpected_Token", p->tokens[node->op_token_index],
	// 2,
	// p), 2);
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	if (!status)
	{
		errno = 0;
		fd = open(node->file, O_RDWR | O_CREAT, 0664);
		handle_open_error(node, p);
		err = errno;
	}
	if (err)
		p->abort = 1;
	if (node->left_node)
	{
		if (!p->abort)
		{
			if (node->in_fd != 0 && parent_is_redir(node, 'i'))
				node->left_node->in_fd = node->in_fd;
			else
				node->left_node->in_fd = fd;
			if (node->out_fd != 1)
				node->left_node->out_fd = node->out_fd;
		}
		if (p->abort && (node->left_node->type == N_CMD
				|| node->left_node->type == N_SUBSHELL))
			return (1);
		status = exec_node(node->left_node, p);
	}
	if (fd != -1 && fd != 0)
		close(fd);
	return (err || status);
}

// int		status;

// status = chk_inf_node_redir_perm(node, p);
// if (!status)
// {
// 	errno = 0;
// 	fd = open(node->file, O_RDONLY);
// 	handle_open_error(node, p);
// 	err = errno;
// }
// if (node->left_node)
// 	status = exec_node(node->left_node, p);
// if (fd != -1)
// 	close(fd);
// return (err || status);

int	exec_herestr_node(t_node *node, t_cmd_set *p)
{
	int		fd;
	char	*filename;
	char	*tmpstr[2];
	int		status;
	int		err;

	status = 0;
	err = 0;
	tmpstr[0] = ft_itoa(node->op_token_index);
	if (!tmpstr[0])
		return (put_err("malloc: Cannot allocate memory", node->file, 1, p), 1);
	tmpstr[1] = ft_strjoin(node->file, tmpstr[0]);
	if (!tmpstr[1])
		return (free(tmpstr[0]), put_err("malloc: Cannot allocate memory",
				node->file, 1, p), 1);
	filename = ft_strjoin("/tmp/", tmpstr[1]);
	// ft_printf_fd(2, "filename herestr node=|%s|\n", filename);
	free_all(tmpstr[0], tmpstr[1], NULL, NULL);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0664);
	if (fd == -1)
		return (put_err("Bad fd", filename, 1, p), free(filename), 1);
	write(fd, (const void *)node->file, ft_strlen(node->file));
	write(fd, "\n", 1);
	if (close(fd) == -1)
		return (put_err("close failed", filename, 1, p), free(filename), 1);
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	if (!status)
	{
		errno = 0;
		fd = open(filename, O_RDONLY);
		handle_open_error(node, p);
		err = errno;
	}
	if (err)
		p->abort = 1;
	// ft_printf_fd(2, "nodefile=~%s~\n", node->file);
	if (node->left_node)
	{
		if (!p->abort)
		{
			if (node->in_fd != 0 && parent_is_redir(node, 'i'))
				node->left_node->in_fd = node->in_fd;
			if (node->out_fd != 1 && node->left_node)
				node->left_node->out_fd = node->out_fd;
			if (!p->abort && node->in_fd == 0)
				node->left_node->in_fd = fd;
		}
		if (p->abort && (node->left_node->type == N_CMD
				|| node->left_node->type == N_SUBSHELL))
			return (1);
		status = exec_node(node->left_node, p);
	}
	if (fd != -1 && fd != 1)
		close(fd);
	return (unlink(filename), free(filename), (err || status));
}

void	remove_token_brackets(char *file)
{
	int	q[2];
	int	i;
	int	j;
	int	init_len;

	// ft_printf_fd(1, "removetokenbrackets in file=~%s~\n", file);
	init_len = ft_strlen(file);
	q[0] = 0;
	q[1] = 0;
	i = -1;
	if (!file)
		return ;
	while (++i < init_len)
	{
		if (file[i] == '\'' && !q[1])
		{
			q[0] = !q[0];
			file[i] = 0;
			j = i - 1;
			while (++j < init_len)
				file[j] = file[j + 1];
			i--;
		}
		else if (file[i] == '\"' && !q[0])
		{
			q[1] = !q[1];
			file[i] = 0;
			j = i - 1;
			while (++j < init_len)
				file[j] = file[j + 1];
			i--;
		}
	}
	// ft_printf_fd(1, "removetokenbrackets out file=~%s~\n", file);
}
//		REIMPLEMENT THIS BUT FROM LEFT TO RIGHT BEFORE EXEC AND STORE
//	PERMISSION STATUS IN A NEW VAR IN NODE SO THAT WHEN EXEC FROM
//	RIGHT TO LEFT THE OPEN() IS CALLED ONLY IF THE VAR IN THE NODE
// BENEATH IS 0;

int	chk_inf_node_redir_perm(t_node *node, t_cmd_set *p)
{
	t_node	*left;
	int		status;

	status = 0;
	left = NULL;
	errno = 0;
	// print_nodetype(node);
	if (node->type != N_READ && node->type != N_WRITE
		&& node->type != N_READWRITE && node->type != N_APPEND)
		return (status);
	if (!(node->file))
		return (-1);
	if (node->type == N_HEREDOC)
		// redundant ? if cond satisfied in the 1st if
		status = check_fd(p->heredoc[node->op_token_index], node, p);
	else
	{
		if (node->type == N_READ || node->type == N_READWRITE)
			status = access(node->file, R_OK);
		if (node->type == N_WRITE || node->type == N_APPEND)
		{
			if (access(node->file, F_OK) == 0)
				status = access(node->file, W_OK);
			else
				status = 0;
		}
	}
	if (status)
		return (status);
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	return (status);
}

int	exec_redirect_node(t_node *node, t_cmd_set *p)
{
	char	**exp;
	char	*file_orig;
	int		quo[3];
	int		i;

	exp = NULL;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	file_orig = ft_strdup(node->file);
	// ft_printf_fd(2, "execredirectnoe file_rrig=%s\n", file_orig);
	node->file = remove_dollar_quote(node->file);
	node->file = var_expander(node->file, quo, p);
	exp = split_and_ignore_space_if_in_quote(node->file, " \t");
	i = -1;
	while (exp[++i])
		exp = glob_expander(&exp, &i, quo, p);
	// print_tab(exp);
	if (ft_arr_len(exp) > 1 || !*node->file)
		return (put_err(file_orig, "ambiguous redirect", 1, p), free(file_orig),
			free_array(&exp), 1);
	// ft_printf_fd(1, "EXEC_REDIRECT_N node->file=%s\n", node->file);
	// if (node->out_fd != 1 && !node->left_node)
	// 	return (close(node->out_fd), 0);
	else
	{
		free(node->file);
		node->file = ft_strdup(exp[0]);
		free_array(&exp);
		free(file_orig);
	}
	if (node->type == N_READ)
		return (exec_read_node(node, p));
	if (node->type == N_HEREDOC)
		return (exec_heredoc_node(node, p));
	if (node->type == N_WRITE || node->type == N_APPEND)
		return (exec_write_or_append_node(node, p));
	if (node->type == N_READWRITE)
		return (exec_readwrite_node(node, p));
	if (node->type == N_HERESTR)
		return (exec_herestr_node(node, p));
	else
		return (-1);
}

void	init_subshell(t_cmd_set *p, t_node *node)
{
	int	i;

	(void)node;
	// char	*new_shlvl;
	// char	*shlvl;
	p->abort = 0;
	// shlvl = ft_getenv("SHLVL", p->envp);
	// new_shlvl = ft_itoa(ft_atoi(shlvl) + 1);
	// p->envp = ft_setenv("SHLVL", new_shlvl, p->envp);
	// free_all(shlvl, new_shlvl, NULL, NULL);
	p->pipe_flag = 0;
	i = -1;
	while (++i < 1024) // void init_pid_arr(t_cmd_set *p)
		p->pid_arr[i] = 0;
	p->pid_index = 0;
	p->shlvl++;
}

void	check_for_heredoc_close_fd(t_node *node, t_cmd_set *p)
{
	if (node->type == N_HEREDOC)
		close(p->heredoc[node->op_token_index]);
	if (node->left_node)
		check_for_heredoc_close_fd(node->left_node, p);
	if (node->right_node)
		check_for_heredoc_close_fd(node->right_node, p);
}

int	exec_subshell_node(t_node *node, t_cmd_set *p)
{
	pid_t	pid;
	int		status;

	status = 0;
	// ft_printf_fd(1, "IN SUBSH\n");
	pid = fork();
	if (pid == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p), 1);
	if (pid > 0)
	{
		p->pid_arr[p->pid_index] = pid;
		p->pid_index++;
		// if (node->in_fd != 0)
		// 	close(node->in_fd);
		// if (node->out_fd != 1)
		// 	close(node->out_fd);
		check_for_heredoc_close_fd(node, p);
		p->pid_of_lst_cmd = pid;
	}
	else if (!pid)
	{
		init_subshell(p, node);
		// increment $SHLVL by 1 reset pid index and pid array
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
		status = exec_node(node->left_node, p);
		// if (node->in_fd != 0)
		// 	close(node->in_fd);
		// if (node->out_fd != 1)
		// 	close(node->out_fd);
		close_fds_except_std();
		return (free_exit(p, p->status_code, NULL), status);
	}
	if (!p->pipe_flag)
		status = wait_and_set_status(p);
	// ft_printf_fd(1, "EXITING SUBSH status=%i\n", status);
	return (status);
}

int	exec_node_and_or(t_node *node, t_cmd_set *p)
{
	int	status;

	// char	*env;
	if (node->in_fd != 0)
	{
		if (node->left_node)
			node->left_node->in_fd = node->in_fd;
		if (node->right_node)
			node->right_node->in_fd = node->in_fd;
	}
	if (node->out_fd != 1)
	{
		if (node->left_node)
			node->left_node->out_fd = node->out_fd;
		if (node->right_node)
			node->right_node->out_fd = node->out_fd;
	}
	p->abort = 0;
	status = exec_node(node->left_node, p);
	// env = ft_getenv("PWD", p->envp);
	// ft_printf_fd(2, "env=|%s|\n", env);
	//	ft_printf_fd(1, "statuscode=%d\n", p->status_code);
	if ((p->status_code == 0 && node->type == N_AND) || (p->status_code != 0
			&& node->type == N_OR))
	{
		p->abort = 0;
		status = exec_node(node->right_node, p);
	} // if (node->in_fd != 0)
		// 	close(node->in_fd);
		// if (node->out_fd != 1)
		// 	close(node->out_fd);
	// ft_printf_fd(1, "EXITING AND_OR status=%i\n", status);
	return (status);
}

int	exec_node(t_node *node, t_cmd_set *p)
{
	if (!node)
		return (-1);
	if (node->type == N_CMD)
		return (exec_cmd_node(node, p));
	else if (node->type == N_OR || node->type == N_AND)
		return (exec_node_and_or(node, p));
	else if (node->type == N_PIPE)
		return (exec_pipe_node(node, p));
	else if (node->type == N_READ || node->type == N_READWRITE
		|| node->type == N_HERESTR || node->type == N_HEREDOC
		|| node->type == N_WRITE || node->type == N_APPEND)
		return (exec_redirect_node(node, p));
	else if (node->type == N_SUBSHELL)
		return (exec_subshell_node(node, p));
	else
		return (-1);
}
