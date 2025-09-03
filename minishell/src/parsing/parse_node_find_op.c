/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node_find_op.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:42:41 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:50:05 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	process_subshell_in_find_op(char **args, int i, t_cmd_set *p)
{
	if (skip_subshell(args, i, -1, p) == -1)
		return (-1);
	return (skip_subshell(args, i, -1, p));
}

int	find_lowest_precedence_op(char **args, t_cmd_set *p)
{
	int	l;
	int	i;
	int	x;
	int	low;

	x = -1;
	low = 6;
	l = ft_arr_len(args);
	i = l;
	while (--i >= 0)
	{
		if (get_nodetype(args[i]) != N_CMD && get_precedence(args[i]) < low)
		{
			low = get_precedence(args[i]);
			x = i;
		}
		if (get_nodetype(args[i]) == N_SUBSHELL)
			i = process_subshell_in_find_op(args, i, p);
	}
	if (low == 6)
		return (0);
	return (x);
}

char	**trim_array(char **args, int i, int j)
{
	char	**newtab;
	int		k;
	int		len;

	k = 0;
	len = ft_arr_len(args);
	if (!(i >= 0 && i <= j && j <= len))
		return (NULL);
	len = j - i + 1;
	newtab = (char **)malloc((len + 1) * sizeof(char *));
	while (k < len)
	{
		newtab[k] = ft_strdup(args[i + k]);
		k++;
	}
	newtab[k] = NULL;
	return (newtab);
}

void	print_tab(char **tab)
{
	int	i;

	ft_printf_fd(1, "\tPRINT_TAB\n");
	i = 0;
	if (!tab)
		ft_printf_fd(1, "\ttab is NULL\n");
	else
	{
		while (tab[i])
		{
			ft_printf_fd(1, "\ttab[%i]=%s\n", i, tab[i]);
			i++;
		}
	}
}

t_cmd	*parse_cmd(char *str, t_cmd_set *p)
{
	t_cmd	*cmd;

	(void)p;
	cmd = init_cmd();
	cmd->line = ft_strdup(str);
	return (cmd);
}
