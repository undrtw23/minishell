/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_checker.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/28 01:01:59 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/08/25 21:14:07 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_builtin(t_cmd *n)
{
	int	len;

	if (!n->args)
		return (0);
	if ((n->args && *n->args && ft_strchr(*n->args, '/')) || (n->cmd_path
			&& ft_strchr(n->cmd_path, '/')))
		return (0);
	len = ft_strlen(*n->args);
	if (len == 0)
		return (0);
	if (!ft_strncmp(*n->args, "pwd", len) && len == 3)
		return (1);
	if (!ft_strncmp(*n->args, "env", len) && len == 3)
		return (1);
	if (!ft_strncmp(*n->args, "cd", len) && len == 2)
		return (1);
	if (!ft_strncmp(*n->args, "export", len) && len == 6)
		return (1);
	if (!ft_strncmp(*n->args, "unset", len) && len == 5)
		return (1);
	if (!ft_strncmp(*n->args, "echo", len) && len == 4)
		return (1);
	if (!ft_strncmp(*n->args, "exit", len) && len == 4)
		return (1);
	return (0);
}
