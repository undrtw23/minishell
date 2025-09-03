/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 01:54:35 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 22:56:26 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**expand_glob(char *glob, t_cmd_set *p)
{
	DIR				*dir;
	struct dirent	*entry;
	char			**exp;

	exp = NULL;
	dir = opendir(".");
	if (!dir)
		put_err("opendir error", NULL, 1, p);
	entry = readdir(dir);
	while (entry != NULL)
	{
		if (should_include_file(entry->d_name, glob))
			exp = ft_array_insert(exp, entry->d_name);
		entry = readdir(dir);
	}
	closedir(dir);
	return (exp);
}

char	**glob_expander(char ***args, int *i, int quo[3], t_cmd_set *p)
{
	char	**exp;
	int		expand;
	int		exp_len;

	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
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
		exp_len = ft_arr_len(exp);
		*i = *i + exp_len - 1;
		free_array(&exp);
	}
	else
		remove_token_brackets(*(*args + *i));
	return (*args);
}
