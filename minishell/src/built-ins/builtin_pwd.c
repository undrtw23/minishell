/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:58:29 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 23:41:35 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Use getcwd to get the current dir then print it. */
int	builtin_pwd_b(void)
{
	char	*buf;

	buf = getcwd(NULL, 0);
	if (!buf)
		return (1);
	ft_putendl_fd(buf, 1);
	free(buf);
	return (0);
}

void	handle_append_operation_helper(char **var_name, char **old_value,
		t_cmd_set *p)
{
	put_err(NULL, "export: variable size exceeds minishell capacity", 1, p);
	free(*var_name);
	free(*old_value);
}
