/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_glob2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 23:01:38 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 02:08:38 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../inc/minishell.h"

static void	advance_heads(t_match_state *ms)
{
	ms->headfile++;
	ms->headglob++;
}

static void	update_indices(t_match_state *ms, char **idxfile, char **idxglob)
{
	*idxfile = ms->headfile;
	*idxglob = ms->headglob;
}

static int	scan_match(char **idxglob, char **idxfile, t_match_state *ms)
{
	int	prev_q_state;

	ms->headglob = *idxglob;
	ms->headfile = *idxfile;
	while (*ms->headglob && *ms->headfile && (*ms->headglob != '*'
			|| ms->quo[2]))
	{
		if (handle_quotes_during_scan(ms, idxglob, &ms->headglob,
				&prev_q_state))
			continue ;
		if (*ms->headglob == '*' && !ms->quo[2])
		{
			*idxglob = ms->headglob;
			continue ;
		}
		if (*ms->headfile != *ms->headglob)
			return (0);
		if (*ms->headfile == *ms->headglob)
			advance_heads(ms);
		ms->headglob = advance_str_past_q(ms->headglob, ms->quo, prev_q_state);
		if (!ms->headfile)
			return (0);
		update_indices(ms, idxfile, idxglob);
	}
	return (1);
}

int match_glob(char *glob, char *file)
{
	char			*idxglob;
	char			*idxfile;
	t_match_state	ms;

	ms.glob = glob;
	ms.file = file;
	init_match_state(&idxglob, &idxfile, &ms);
	while (*idxglob)
	{
		ms.skip = skip_stars(&idxglob, ms.quo[2]);
		if (!*idxglob)
			return (1);
		if (*idxglob && !*idxfile)
			return (0);
		ms.headfile = idxfile;
		ms.headglob = idxglob;
		store_head_state(&ms);
		if (!scan_match(&idxglob, &idxfile, &ms))
		{
			idxfile++;
			reset_quote_state(&ms);
		}
	}
	return (1);
}
