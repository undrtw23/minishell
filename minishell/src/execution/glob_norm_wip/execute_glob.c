/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_glob.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 23:01:38 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 02:08:33 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//#include "../../inc/minishell.h"

int	find_glob(char *token, int *quo)
{
	while (*token)
	{
		upd_quo(quo, *token);
		if (!quo[2] && *token == '*')
			return (1);
		token++;
	}
	return (0);
}

char	*advance_str_past_q(char *headglob, int *quo, int prev_q_state)
{
	while (*headglob && (*headglob == '\'' || *headglob == '\"'))
	{
		prev_q_state = quo[2];
		upd_quo(quo, *headglob);
		headglob++;
	}
	return (headglob);
}

int	match_glob(char *glob, char *file)
{
	char	*idxglob;
	char	*idxfile;
	char	*headfile;
	char	*headglob;
	int		quo[3];
	int		skip;
	int		prev_q_state;
	int		headqstate[3];

	prev_q_state = 0;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	idxglob = glob;
	idxfile = file;
	skip = 0;
	while (*idxglob)
	{
		skip = 0;
		while (*idxglob && !quo[2] && *idxglob == '*')
		{
			idxglob++;
			skip = 1;
		}
		if (!*idxglob)
			return (1);
		if (*idxglob && !*idxfile)
			return (0);
		headfile = idxfile;
		headglob = idxglob;
		headqstate[0] = quo[0];
		headqstate[1] = quo[1];
		headqstate[2] = quo[2];
		while (*headglob && *headfile && (*headglob != '*' || quo[2]))
		{
			prev_q_state = quo[2];
			upd_quo(quo, *headglob);
			if (quo[2] != prev_q_state && *(headglob + 1))
			{
				headglob++;
				idxglob++;
				continue ;
			}
			if (*headglob == '*' && !quo[2])
			{
				idxglob = headglob;
				continue ;
			}
			if (skip)
			{
				while (headfile)
				{
					headfile = ft_strchr(headfile, *headglob);
					if (!headfile)
						return (0);
					while (*headfile && *headglob && (*headglob != '*'
							|| quo[2]) && (*headfile == *headglob))
					{
						headglob++;
						headfile++;
						if (!*headglob && *headfile && ((headglob
									- idxglob) == 1))
						{
							headglob = idxglob;
							headfile = ft_strchr(headfile, *headglob);
							continue ;
						}
						headglob = advance_str_past_q(headglob, quo,
								prev_q_state);
					}
					if (!*headfile || !*headglob || (*headglob == '*'
							&& !quo[2]))
						break ;
					else
					{
						headglob = idxglob;
						headfile++;
						headfile = ft_strchr(headfile, *headglob);
					}
				}
				skip = 0;
			}
			else if (*headfile != *headglob)
				return (0);
			else if (*headfile == *headglob)
			{
				headfile++;
				headglob++;
			}
			headglob = advance_str_past_q(headglob, quo, prev_q_state);
			if (!headfile)
				return (0);
			if (!*headglob)
			{
				if (!*headfile)
					return (1);
				else
				{
					headglob = idxglob;
					quo[0] = headqstate[0];
					quo[1] = headqstate[1];
					quo[2] = headqstate[2];
					continue ;
				}
			}
			idxfile = headfile;
			idxglob = headglob;
		}
	}
	return (1);
}

char	**expand_glob(char *glob, t_cmd_set *p)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**exp;
	char			*file[2];

	exp = NULL;
	dir = opendir(".");
	if (!dir)
		put_err("opendir error", NULL, 1, p);
	(void)p;
	entry = readdir(dir);
	while (entry != NULL)
	{
		file[0] = NULL;
		file[0] = entry->d_name;
		entry = readdir(dir);
		if (file[0] && file[0][0] == '.')
			continue ;
		if (match_glob(glob, file[0]))
			exp = ft_array_insert(exp, file[0]);
	}
	closedir(dir);
	return (exp);
}

char	**glob_expander(char ***args, int *i, int quo[3], t_cmd_set *p)
{
	char	**exp;
	int		expand;

	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	(void)p;
	expand = 0;
	exp = NULL;
	expand = find_glob(*(*args + *i), quo);
	if (ft_strcmp("export", *(*args)) && expand)
	{
		exp = expand_glob(*(*args + *i), p);
		if (!exp)
		{
			remove_token_brackets(*(*args + *i));
			return (*args);
		}
		*args = ft_array_replace(args, exp, *i);
		*i = *i + ft_arr_len(exp) - 1;
		free_array(&exp);
	}
	else
		remove_token_brackets(*(*args + *i));
	return (*args);
}
