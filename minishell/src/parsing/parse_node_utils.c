/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_node_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:42:51 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:44:43 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_subshell_error(char **args, int old_i, t_cmd_set *p)
{
	if (!ft_strncmp(args[old_i], "(", 1))
	{
		put_err("Unexpected_Token", "\\n", 2, p);
		return (-1);
	}
	if (args[old_i - 1] && get_precedence(args[old_i - 1]) >= 2 && *args[old_i
			- 1] != ')')
	{
		put_err("Unexpected_Token", args[old_i], 2, p);
		return (-1);
	}
	return (0);
}

int	check_subshell_forward_error(char **args, int i, int old_i, t_cmd_set *p)
{
	if (args[i + 1] && get_precedence(args[i + 1]) >= 2 && *args[i + 1] != '(')
	{
		put_err("Unexpected_Token", args[old_i], 2, p);
		return (-1);
	}
	return (i);
}

int	skip_subshell_backward(char **args, int i, t_cmd_set *p)
{
	int	k;
	int	old_i;

	old_i = i;
	k = 0;
	if (handle_subshell_error(args, old_i, p) == -1)
		return (-1);
	while (i >= 0 && args[i])
	{
		if (!ft_strncmp(args[i], ")", 1))
			k--;
		else if (!ft_strncmp(args[i], "(", 1))
			k++;
		if (k == 0)
			return (check_subshell_forward_error(args, i, old_i, p));
		i--;
	}
	put_err("Unexpected_Token", args[old_i], 2, p);
	return (-1);
}

int	skip_subshell_forward(char **args, int i, int l)
{
	int	k;

	k = 0;
	while (i < l && args[i])
	{
		if (!ft_strncmp(args[i], ")", 1))
			k--;
		else if (ft_strncmp(args[i], "(", 1))
			k++;
		if (k == 0)
			return (i);
		i++;
	}
	return (-1);
}

int	skip_subshell(char **args, int i, int direction, t_cmd_set *p)
{
	int	l;
	int	old_i;

	old_i = i;
	l = ft_strlen(args[i]);
	if (direction == -1)
		return (skip_subshell_backward(args, i, p));
	else
		return (skip_subshell_forward(args, i, l));
}
