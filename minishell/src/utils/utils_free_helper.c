/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free_helper.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:37:12 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/04 17:11:44 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Frees temporary data structures used during parsing failure.
Clears the command list and both string arrays.
Returns NULL to allow inline usage on error. */
t_list	*free_tmp_lst(t_list *cmds, char **args, char **temp)
{
	ft_lstclear(&cmds, free_lst);
	free_array(&temp);
	free_array(&args);
	return (NULL);
}

/* Brut force function to close all those pesky FD's.
Make sur that execve only inherit 0, 1, 2.*/
void	close_fds_except_std(void)
{
	int	i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}
