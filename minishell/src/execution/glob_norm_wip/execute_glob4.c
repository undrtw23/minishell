/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_glob4.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 23:01:38 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 02:08:45 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../inc/minishell.h"

static void	reset_quote_state(t_match_state *ms)
{
	ms->quo[0] = ms->headq[0];
	ms->quo[1] = ms->headq[1];
	ms->quo[2] = ms->headq[2];
}

static void	store_head_state(t_match_state *ms)
{
	ms->headq[0] = ms->quo[0];
	ms->headq[1] = ms->quo[1];
	ms->headq[2] = ms->quo[2];
}

static int handle_quotes_during_scan(t_match_state *ms, char **idxglob,
		char **headglob, int *prev_q_state)
{
	*prev_q_state = ms->quo[2];
	upd_quo(ms->quo, **headglob);
	if (ms->quo[2] != *prev_q_state && *(*headglob + 1))
	{
		(*headglob)++;
		(*idxglob)++;
		return (1);
	}
	return (0);
}

