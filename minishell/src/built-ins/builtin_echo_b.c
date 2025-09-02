/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo_b.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 23:57:40 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/08/27 22:22:24 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Used in builtin echo to count the number of characters that are 'n' in an
argument of function echo.
ex : "echo -nnnn" : c_count = 4 = ft_strlen(arg[1]) - 1*/
int	c_count(char *s, char c)
{
	int	count;

	if (!s)
		return (0);
	count = 0;
	while (*s)
	{
		if (*s == c)
			count++;
		s++;
	}
	return (count);
}

/* First check if -n (or -nnnn is present) via c_count and flag print_newline
j is flag to check if we started printing or not. Start at arg[1].
Print every arg with one and only one space between them.*/
int	builtin_echo_b(t_cmd *cmd)
{
	int		print_newline;
	int		i;
	int		j;
	char	**args;

	i = 0;
	j = 0;
	//print_tab(cmd->args);
	print_newline = 1;
	args = cmd->args;
	while (args && args[++i])
	{
		if (!j && !ft_strncmp(args[i], "-n", 2) && (c_count(args[i],
					'n') == (int)(ft_strlen(args[i]) - 1)))
			print_newline = 0;
		else
		{
			j = 1;
			ft_printf_fd(1, "%s", args[i]);
			if (args[i + 1])
				ft_putchar_fd(' ', 1);
		}
	}
	if (print_newline)
		write(1, "\n", 1);
	return (0);
}
