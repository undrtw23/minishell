/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_path_helper.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/29 12:17:58 by cwang             #+#    #+#             */
/*   Updated: 2025/09/02 22:53:49 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* If path is empty make sur the is something in it
Control memory leaks when exporting empty $PATH*/
char	*ensure_path(char *path)
{
	if (!path || !*path)
	{
		free(path);
		return (ft_strdup("/"));
	}
	return (path);
}

/* Helper for find command path that handle errors.*/
void	handle_exec_err(t_cmd *n, DIR *dir, t_cmd_set *p)
{
	int	slash;

	(void)dir;
	if (!n || is_builtin(n))
		return ;
	slash = 0;
	if (n->args && n->args[0])
		slash = ft_strchr("/~", n->args[0][0]) != NULL;
	if (n->args && n->args[0][0] && n->args[0][0] == '.'
		&& ft_strlen(n->args[0]) == 1)
		put_err(".", ".", 2, p);
	else if (n->args && n->args[0][0] && slash && dir)
		put_err("Is_Directory", *n->args, 126, p);
	else if (n->cmd_path && access(n->cmd_path, F_OK) == -1)
		put_err("NoFile_NoDir", n->cmd_path, 127, p);
	else if (n->cmd_path && access(n->cmd_path, X_OK) == -1)
		put_err("Perm_Denied", n->cmd_path, 126, p);
}
