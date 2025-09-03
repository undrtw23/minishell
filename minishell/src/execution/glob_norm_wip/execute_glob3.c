/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_glob3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 23:01:38 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 02:08:42 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../inc/minishell.h"

static void	init_match_state(char **idxglob, char **idxfile, t_match_state *ms)
{
	*idxglob = glob;
	*idxfile = file;
	ms->quo[0] = 0;
	ms->quo[1] = 0;
	ms->quo[2] = 0;
}

static int	skip_stars(char **idxglob, int quoted)
{
	int	skip;

	skip = 0;
	while (**idxglob && !quoted && **idxglob == '*')
	{
		(*idxglob)++;
		skip = 1;
	}
	return (skip);
}