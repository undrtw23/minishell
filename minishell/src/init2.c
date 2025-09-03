/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 22:36:13 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 04:46:45 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_pid_arr(t_cmd_set *p)
{
	int	i;

	i = -1;
	while (++i < 4096)
		p->pid_arr[i] = 0;
}

void	init_heredoc_arr(t_cmd_set *p)
{
	int	i;

	i = -1;
	while (++i < 4096)
		p->heredoc[i] = -1;
}
