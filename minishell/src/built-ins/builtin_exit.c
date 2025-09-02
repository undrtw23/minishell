/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:35:53 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/07/30 21:10:23 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Parse digits from a numeric string
Safely accumulates digits into an unsigned long while checking
for overflows based on the given sign. Returns 0 on overflow. */
int	parse_digits(const char *str, int *i, unsigned long *n, int sign)
{
	while (str[*i] && ft_isdigit(str[*i]))
	{
		*n = *n * 10 + (str[*i] - '0');
		if ((sign == 1 && *n > LLONG_MAX) || (sign == -1
				&& *n > (unsigned long)LLONG_MAX + 1))
			return (0);
		(*i)++;
	}
	return (1);
}

/* Strict string-to-integer parser
Parses a string into a long long while enforcing numeric-only format.
Handles optional +/-, checks for overflow, and returns 0 if invalid. */
int	parse_strict_ll(const char *str, long long *out)
{
	int				sign;
	int				i;
	unsigned long	n;

	i = 0;
	n = 0;
	sign = 1;
	if (!str || !str[0])
		return (0);
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	if (!ft_isdigit(str[i]))
		return (0);
	if (!parse_digits(str, &i, &n, sign))
		return (0);
	if (str[i])
		return (0);
	*out = sign * (long long)n;
	return (1);
}

/* Builtin exit for parent context
Handles `exit` command with numeric validation, overflow detection,
error messaging, and proper status code truncation. */
void	builtin_exit(t_list *cmd, t_cmd_set *p)
{
	long long	status;
	char		**args;
	int			is_exit;

	is_exit = 0;
	args = ((t_cmd *)cmd->content)->args;
	ft_putendl_fd("\033[1;31mexit\033[0m", 1);
	if (!args || !args[1])
		free_exit(p, 0, NULL);
	if (!parse_strict_ll(args[1], &status))
	{
		ft_printf_fd(2, "mini: exit: %s: numeric argument required\n", args[1]);
		free_exit(p, 2, NULL);
	}
	if (args[2])
	{
		put_err(NULL, "exit: too many arguments", 1, 0);
		is_exit = 1;
	}
	if (is_exit == 0)
		free_exit(p, status & 255, NULL);
}

/* Builtin exit for child process
Validates argument format and overflow, returns proper exit code,
and reports errors as needed without exiting the program. */
int	builtin_exit_child(t_list *cmd)
{
	long long	status;
	char		**args;

	args = ((t_cmd *)cmd->content)->args;
	if (!args || !args[1])
		return (0);
	if (!parse_strict_ll(args[1], &status))
	{
		ft_printf_fd(2, "mini: exit: %s: numeric argument required\n", args[1]);
		return (2);
	}
	if (args[2])
	{
		ft_putendl_fd("mini: exit: too many arguments", 2);
		return (1);
	}
	return (status & 255);
}
