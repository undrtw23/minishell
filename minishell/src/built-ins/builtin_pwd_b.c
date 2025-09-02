/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd_b.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:58:29 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/07/15 22:42:18 by alsima           ###   ########.fr       */
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
