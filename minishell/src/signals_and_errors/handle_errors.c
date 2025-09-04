/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_errors.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:32:55 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/04 22:40:04 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	*put_err_helper(char *err_msg, char *cmd, t_cmd_set *p)
{
	if (!ft_strncmp(err_msg, "No_Cmd", 6) && p->status_code != 2)
		ft_printf_fd(2, "mini: %s: command not found\n", cmd);
	else if (!ft_strncmp(err_msg, "DupForkPipe_Failed", 18))
		ft_printf_fd(2, "mini: %s: dup2,fork, open or pipe failed\n", cmd);
	else if (!ft_strncmp(err_msg, "Is_Directory", 12))
		ft_printf_fd(2, "mini: %s: Is a directory\n", cmd);
	else if (!ft_strncmp(err_msg, "Is_Not_Directory", 16))
		ft_printf_fd(2, "mini: %s: A path component is not a directory\n", cmd);
	else if (!ft_strncmp(err_msg, "Loop_Links", 10))
		ft_printf_fd(2, "mini: %s: Too many symbolic links\n", cmd);
	else if (!ft_strncmp(err_msg, "Unexpected_Token", 16))
		error_unexpected_token(cmd);
	else if (!ft_strncmp(err_msg, "close failed", 12))
		ft_printf_fd(2, "mini: %s: %s\n", err_msg, cmd);
	else
		ft_printf_fd(2, "mini: %s: %s\n", err_msg, cmd);
	return (NULL);
}

/* Prints an appropriate error message to stderr based on err_msg.
Updates the shell's status code and global exit status.
Returns NULL to allow usage in error-handling expressions. */
void	*put_err(char *err_msg, char *cmd, int err_code, t_cmd_set *p)
{
	if (p && err_code >= 0)
		p->status_code = err_code;
	g_exit_status = err_code;
	if (err_msg)
	{
		if (!ft_strncmp(err_msg, "Unclosed_Quote", 14))
			error_unclosed_quotes();
		else if (err_msg && !ft_strncmp(err_msg, ".", 12))
		{
			ft_printf_fd(2, "mini: %s: filename argument required\n.: ", cmd);
			ft_printf_fd(2, "usage:.filename[arguments]\n ", cmd);
		}
		else if (err_msg && !ft_strncmp(err_msg, "NoFile_NoDir", 12))
			ft_printf_fd(2, "mini: %s: No such file or directory\n", cmd);
		else if (!ft_strncmp(err_msg, "Perm_Denied", 11))
			ft_printf_fd(2, "mini: %s: Permission denied\n", cmd);
		else
			put_err_helper(err_msg, cmd, p);
	}
	else if (cmd && cmd[0] != '\0')
		ft_printf_fd(2, "mini: %s\n", cmd);
	return (NULL);
}

void	*put_err_cd(char *err_type, char *path, int code, t_cmd_set *p)
{
	if (p && code >= 0)
		p->status_code = code;
	g_exit_status = code;
	if (err_type && !ft_strncmp(err_type, "HOME_Not_Set", 12))
	{
		ft_putendl_fd("mini: cd: HOME not set", 2);
		return (NULL);
	}
	if (err_type && !ft_strncmp(err_type, "OLDPWD_Not_Set", 14))
	{
		ft_putendl_fd("mini: cd: OLDPWD not set", 2);
		return (NULL);
	}
	if (!err_type || !path || !path[0])
		return (NULL);
	if (!ft_strncmp(err_type, "NoFile_NoDir", 12))
		ft_printf_fd(2, "mini: cd: %s: No such file or directory\n", path);
	else if (!ft_strncmp(err_type, "Perm_Denied", 11))
		ft_printf_fd(2, "mini: cd: %s: Permission denied\n", path);
	else if (!ft_strncmp(err_type, "Not_Directory", 13))
		ft_printf_fd(2, "mini: cd: %s: Not a directory\n", path);
	return (NULL);
}

/* Same for syntax errors.*/
void	*put_err_syntax(char *err_msg, int err_code, t_cmd_set *p)
{
	if (p && err_code >= 0)
		p->status_code = err_code;
	g_exit_status = err_code;
	if (err_msg)
		if (!ft_strncmp(err_msg, "eof", 3))
			ft_printf_fd(2, "mini: syntax error: unexpected end of file\n");
	return (NULL);
}
