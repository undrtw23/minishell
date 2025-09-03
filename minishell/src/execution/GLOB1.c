/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   GLOB1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:54:35 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 05:10:55 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// int	find_glob(char *token, int *quo)
// {
// 	while (*token)
// 	{
// 		upd_quo(quo, *token);
// 		if (!quo[2] && *token == '*')
// 			return (1);
// 		token++;
// 	}
// 	return (0);
// }

// void	init_glob_vars(t_glob_match *m, char *glob, char *file)
// {
// 	m->glob = glob;
// 	m->file = file;
// 	m->idxglob = glob;
// 	m->idxfile = file;
// 	m->quo[0] = 0;
// 	m->quo[1] = 0;
// 	m->quo[2] = 0;
// }

// void	set_head_vars(t_glob_match *m)
// {
// 	m->headfile = m->idxfile;
// 	m->headglob = m->idxglob;
// 	m->headqstate[0] = m->quo[0];
// 	m->headqstate[1] = m->quo[1];
// 	m->headqstate[2] = m->quo[2];
// }

// int	skip_asterisks(t_glob_match *m)
// {
// 	int	skip;

// 	skip = 0;
// 	while (*m->idxglob && !m->quo[2] && *m->idxglob == '*')
// 	{
// 		m->idxglob++;
// 		skip = 1;
// 	}
// 	return (skip);
// }

// int	handle_skip_case(t_glob_match *m)
// {
// 	char	*temp_glob;
// 	char	*temp_file;
// 	int		prev_q_state;

// 	temp_glob = m->headglob;
// 	temp_file = m->headfile;
// 	prev_q_state = m->quo[2];
// 	while (temp_file)
// 	{
// 		temp_file = ft_strchr(temp_file, *temp_glob);
// 		if (!temp_file)
// 			return (0);
// 		while (*temp_file && *temp_glob && (*temp_glob != '*' || m->quo[2])
// 			&& (*temp_file == *temp_glob))
// 		{
// 			temp_glob++;
// 			temp_file++;
// 			if (!*temp_glob && *temp_file && ((temp_glob - m->idxglob) == 1))
// 			{
// 				temp_glob = m->idxglob;
// 				temp_file = ft_strchr(temp_file, *temp_glob);
// 				continue ;
// 			}
// 			temp_glob = advance_str_past_q(temp_glob, m->quo, prev_q_state);
// 		}
// 		if (!*temp_file || !*temp_glob || (*temp_glob == '*' && !m->quo[2]))
// 			break ;
// 		else
// 		{
// 			temp_glob = m->idxglob;
// 			temp_file++;
// 			temp_file = ft_strchr(temp_file, *temp_glob);
// 		}
// 	}
// 	m->headglob = temp_glob;
// 	m->headfile = temp_file;
// 	return (1);
// }

// int	process_char_match(t_glob_match *m, int prev_q_state)
// {
// 	if (*m->headfile != *m->headglob)
// 		return (0);
// 	else if (*m->headfile == *m->headglob)
// 	{
// 		m->headfile++;
// 		m->headglob++;
// 	}
// 	m->headglob = advance_str_past_q(m->headglob, m->quo, prev_q_state);
// 	return (1);
// }

// int	handle_pattern_match(t_glob_match *m)
// {
// 	int	prev_q_state;

// 	while (*m->headglob && *m->headfile && (*m->headglob != '*' || m->quo[2]))
// 	{
// 		prev_q_state = m->quo[2];
// 		upd_quo(m->quo, *m->headglob);
// 		if (m->quo[2] != prev_q_state && *(m->headglob + 1))
// 		{
// 			m->headglob++;
// 			m->idxglob++;
// 			continue ;
// 		}
// 		if (*m->headglob == '*' && !m->quo[2])
// 		{
// 			m->idxglob = m->headglob;
// 			return (1);
// 		}
// 		if (!process_char_match(m, prev_q_state))
// 			return (0);
// 		// if (!m->headglob)
// 		// 	return (0);
// 	}
// 	m->idxfile = m->headfile;
// 	m->idxglob = m->headglob;
// 	return (1);
// }

// char	*advance_str_past_q(char *headglob, int *quo, int prev_q_state)
// {
// 	while (*headglob && (*headglob == '\'' || *headglob == '\"'))
// 	{
// 		prev_q_state = quo[2];
// 		upd_quo(quo, *headglob);
// 		headglob++;
// 	}
// 	return (headglob);
// }

// int	match_glob(char *glob, char *file)
// {
// 	t_glob_match	m;
// 	int				skip;

// 	init_glob_vars(&m, glob, file);
// 	while (*m.idxglob)
// 	{
// 		skip = skip_asterisks(&m);
// 		if (!*m.idxglob)
// 			return (1);
// 		if (*m.idxglob && !*m.idxfile)
// 			return (0);
// 		set_head_vars(&m);
// 		if (!handle_pattern_match(&m))
// 			return (0);
// 	}
// 	return (1);
// }

// char	**expand_glob(char *glob, t_cmd_set *p)
// {
// 	DIR				*dir;
// 	struct dirent	*entry;
// 	char			**exp;
// 	char			*file[2];

// 	exp = NULL;
// 	dir = opendir(".");
// 	if (!dir)
// 		put_err("opendir error", NULL, 1, p);
// 	(void)p;
// 	entry = readdir(dir);
// 	while (entry != NULL)
// 	{
// 		file[0] = NULL;
// 		file[0] = entry->d_name;
// 		entry = readdir(dir);
// 		if (file[0] && file[0][0] == '.')
// 			continue ;
// 		if (match_glob(glob, file[0]))
// 			exp = ft_array_insert(exp, file[0]);
// 	}
// 	closedir(dir);
// 	return (exp);
// }

// char	**glob_expander(char ***args, int *i, int quo[3], t_cmd_set *p)
// {
// 	char	**exp;
// 	int		expand;

// 	quo[0] = 0;
// 	quo[1] = 0;
// 	quo[2] = 0;
// 	(void)p;
// 	expand = 0;
// 	exp = NULL;
// 	expand = find_glob(*(*args + *i), quo);
// 	if (ft_strcmp("export", *(*args)) && expand)
// 	{
// 		exp = expand_glob(*(*args + *i), p);
// 		if (!exp)
// 		{
// 			remove_token_brackets(*(*args + *i));
// 			return (*args);
// 		}
// 		*args = ft_array_replace(args, exp, *i);
// 		*i = *i + ft_arr_len(exp) - 1;
// 		free_array(&exp);
// 	}
// 	else
// 		remove_token_brackets(*(*args + *i));
// 	return (*args);
// }

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
	// ft_printf_fd(2, "matchglob glob=%s file=%s\n", glob, file);
	while (*idxglob)
	{
		// ft_printf_fd(2, "	matchglob start idxglob=%s idxfile=%s\n", idxglob,
		// 		idxfile);
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
			// ft_printf_fd(2,
			// 	"matchglob mid hglob=%s hfile=%s quo[2]=%i prevqs=%i\n",
			// 	headglob, headfile, quo[2], prev_q_state);
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
			// ft_printf_fd(2,
			// 	"matchglob beforeskip hglob=%s hfile=%s quo[2]=%i prevqs=%i\n",
			// 	headglob, headfile, quo[2], prev_q_state);
			if (skip)
			{
				while (headfile)
				{
					headfile = ft_strchr(headfile, *headglob);
					if (!headfile)
						return (0);
					while (headfile && *headfile && *headglob && (*headglob != '*'
							|| quo[2]) && (*headfile == *headglob))
					{
						// ft_printf_fd(2, "skipin2ndwhile hglob=%s hfile=%s\n",
						// 	headglob, headfile);
						headglob++;
						headfile++;
						if (!*headglob && *headfile && ((headglob
									- idxglob) == 1))
						{
							headglob = idxglob;
							headfile = ft_strchr(headfile, *headglob);
							// ft_printf_fd(2,
							// 	"matchglob when headglob is0 but headfile not over hglob=%s hfile=%s\n",
							// 	headglob, headfile);
							continue ;
						}
						headglob = advance_str_past_q(headglob, quo,
								prev_q_state);
						// ft_printf_fd(2,
						// 	"matchglob after advancestrpastq headglob=%s\n",
						// 	headglob);
					}
					// ft_printf_fd(2, "skipafter2ndwhile hglob=%s hfile=%s\n",
					// 	headglob, headfile);
					if (!headfile || !*headfile || !*headglob || (*headglob == '*'
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
			// ft_printf_fd(2, "	matchglob headglob=%s headfile=%s\n", headglob,
			// 		headfile);
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
	// ft_printf_fd(2, "expandglob glob=%s\n", glob);
	// remove_token_brackets(glob);
	// ft_printf_fd(2, "expandglob glob=%s\n", glob);
	while (entry != NULL)
	{
		file[0] = NULL;
		file[0] = entry->d_name;
		entry = readdir(dir);
		if (file[0] && file[0][0] == '.' && (glob[0] != '.'))
			continue ;
		// ft_printf_fd(2, "expandglob glob=%s, file=%s\n", glob, file[0]);
		if (match_glob(glob, file[0]))
			exp = ft_array_insert(exp, file[0]);
	}
	closedir(dir);
	return (exp);
}

char	**glob_expander(char ***args, int *i, int quo[3], t_cmd_set *p)
{
	char **exp;
	int expand;

	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	(void)p;
	expand = 0;
	exp = NULL;
	// print_tab(*args);
	// ft_printf_fd(2, " before fndglob args[%i]=%s\n", (int)(*i), *(*args
	// + *i));
	expand = find_glob(*(*args + *i), quo);
	// ft_printf_fd(2, "globexpander token=%s expand=%i\n", *(*args + *i),
	// expand);
	if (ft_strcmp("export", *(*args)) && expand)
	{
		exp = expand_glob(*(*args + *i), p);
		// print_tab(exp);
		if (!exp)
		{
			// ft_printf_fd(2, "globexpander glob 0 finds glob=%s\n",
			// *(*args + *i));
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