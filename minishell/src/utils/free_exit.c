/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:28 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/01 18:20:44 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Exit and cleaning function. Can print something if needed.
Free everything that is allocated in the main struct :
- p->envp as an **array
- p->cmd as a command list (see lst_clear)
- p->input_text as a *char.*/
void	free_exit(t_cmd_set *p, int exit_code, char *msg)
{
	int	i;

	i = -1;
	(void)msg;
//	ft_printf_fd(1, "ugh1\n");
	// if (msg)
	// 	ft_printf_fd(2, msg);
//	ft_printf_fd(1, "ugh2\n");
	if (p && p->tokens)
		free_array(&p->tokens);
//	ft_printf_fd(1, "ugh3\n");
	if (p && p->input_text)
		free_array(&(p->input_text));
//	ft_printf_fd(1, "ugh4\n");
	if (p && p->filename)
		free(p->filename);
//	ft_printf_fd(1, "ugh5\n");
	if (p && p->envp && *p->envp)
		free_array(&p->envp);
//	ft_printf_fd(1, "ugh6\n");
	if (p && p->nodes)
		free_node(p->nodes);
//	ft_printf_fd(1, "ugh7\n");
	rl_clear_history();
//	ft_printf_fd(1, "ugh8\n");
	exit(exit_code);
}
