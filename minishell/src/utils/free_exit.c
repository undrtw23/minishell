/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 19:23:28 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:29:33 by ngaurama         ###   ########.fr       */
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
	if (p && p->tokens)
		free_array(&p->tokens);
	if (p && p->input_text)
		free_array(&(p->input_text));
	if (p && p->filename)
		free(p->filename);
	if (p && p->envp && *p->envp)
		free_array(&p->envp);
	if (p && p->nodes)
		free_node(p->nodes);
	rl_clear_history();
	exit(exit_code);
}
