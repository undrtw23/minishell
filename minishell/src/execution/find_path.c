/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:31:19 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/02 22:54:26 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*Duplicate dir into temp_dir, append "/" to it
then append the command.
free temp and return cmd_path*/
static char	*build_cmd_path(const char *dir, const char *cmd)
{
	char	*temp_dir;
	char	*temp;
	char	*cmd_path;

	temp_dir = ft_strdup(dir);
	if (!temp_dir)
		return (NULL);
	temp = ft_strjoin(temp_dir, "/");
	free(temp_dir);
	if (!temp)
		return (NULL);
	cmd_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!cmd_path)
		return (NULL);
	return (cmd_path);
}

/* Loops through env variable to check if the command is in a path of $PATH
Call build command path each time (append '/' cmd)
so it search for access to cmd_path and if execution is possible.
Free the path each time to check for next one.
if nothing is found in $PATH fall back to "." which is current directory.
lately if nothing found return NULL and launch error no_cmd in process_check*/
char	*find_command(char **env_path, char *cmd, char *cmd_path)
{
	int	i;

	i = -1;
	while (env_path && env_path[++i])
	{
		free_all(cmd_path, NULL, NULL, NULL);
		cmd_path = build_cmd_path(env_path[i], cmd);
		if (!cmd_path)
			return (NULL);
		if (access(cmd_path, X_OK) == 0)
			return (cmd_path);
	}
	free_all(cmd_path, NULL, NULL, NULL);
	cmd_path = build_cmd_path(".", cmd);
	if (cmd_path && access(cmd_path, X_OK) == 0)
		return (cmd_path);
	free_all(cmd_path, NULL, NULL, NULL);
	return (NULL);
}

/* check a naked command which is not a builtin (type ls)
if empty (ex : "   ") error :No_CMD
Split the $PATH variable into an array:
[0] : /usr/bin
[1] : /usr/sbin
....
call find command
if nothing found and last command error code 127
else 0 (executed in child)*/
static void	process_checks(t_node *node, char *path, t_cmd_set *p, char ***s)
{
	t_cmd	*n;

	n = node->cmd;
	if (n->args && n->args[0] && n->args[0][0] == '\0')
	{
		put_err("No_Cmd", n->args[0], 127, p);
		return ;
	}
	if (!is_builtin(n) && n->args && !ft_strchr("/~", n->args[0][0]))
	{
		*s = ft_split(path, ':');
		n->cmd_path = find_command(*s, *n->args, n->cmd_path);
		if ((!n->cmd_path || !n->args[0]))
			put_err("No_Cmd", n->args[0], 127, p);
		else
		{
			free(n->args[0]);
			n->args[0] = n->cmd_path;
		}
	}
}

/* Try opendir to check if the cmd entered is a directory
if not and contain a '/' store a copy into cmd_path
and replace arg[0] with the command /bin/ls -> ls
if not fallback to process_check.
if the cmd is not a directory, dir is null and conditions in
find_cmd_path will be false.*/
static DIR	*cmd_checks(t_node *node, char ***s, char *path, t_cmd_set *p)
{
	t_cmd	*n;
	DIR		*dir;

	n = node->cmd;
	dir = NULL;
	if (n && n->args)
		dir = opendir(n->args[0]);
	if (n && n->args && ft_strchr(*n->args, '/') && !dir)
	{
		*s = ft_split(*n->args, '/');
		n->cmd_path = ft_strdup(*n->args);
		free(n->args[0]);
		n->args[0] = ft_strdup((*s)[ft_arr_len(*s) - 1]);
	}
	else if (!ft_strchr(*n->args, '/') && !dir)
		process_checks(node, path, p, s);
	free_all(path, NULL, NULL, NULL);
	return (dir);
}

/*Main cmd_path_checker.
Throw errors in function of cmd_check.
First get the env variable PATH from p->envp
Close dir if opened before (non NULL).*/
void	find_cmd_path(t_cmd_set *p, t_node *node, char *path)
{
	t_cmd	*n;
	DIR		*dir;
	char	**s;

	s = NULL;
	n = node->cmd;
	path = ft_getenv("PATH", p->envp);
	path = ensure_path(path);
	dir = cmd_checks(node, &s, path, p);
	handle_exec_err(n, dir, p);
	if (dir)
		closedir(dir);
	if (node->cmd->args[0][0])
	{
		if (s)
			free_array(&s);
	}
}
