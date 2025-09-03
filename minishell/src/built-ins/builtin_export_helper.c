/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_helper.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 19:52:18 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 20:03:12 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	var_in_envp(char *arg, char **envp, int *idx)
{
	int	pos;

	*idx = 0;
	pos = char_index(arg, '=');
	if (pos == -1)
		return (-1);
	while (envp[*idx])
	{
		if (!ft_strncmp(envp[*idx], arg, pos + 1))
			return (1);
		(*idx)++;
	}
	return (0);
}

int	validate_append_identifier(char *str, int pos)
{
	int	i;

	i = 0;
	while (i < pos - 1)
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (ft_printf_fd(2,
					"mini: export: `%s': not a valid identifier\n", str), 0);
		i++;
	}
	return (1);
}

int	validate_regular_identifier(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (ft_printf_fd(2,
					"mini: export: `%s': not a valid identifier\n", str), 0);
		i++;
	}
	return (1);
}

int	is_valid_identifier(char *str)
{
	int	pos;

	if (!str || str[0] == '\0')
		return (ft_printf_fd(2, "mini: export: `%s': not a valid identifier\n",
				str), 0);
	if (str[0] == '-')
	{
		if (ft_strcmp(str, "--") == 0)
			return (1);
		if (ft_strcmp(str, "-n") == 0 || ft_strcmp(str, "-f") == 0)
			return (1);
		return (ft_printf_fd(2, "mini: export: `%s': invalid option\n", str),
			0);
	}
	if (ft_isdigit(str[0]) || str[0] == '=')
		return (ft_printf_fd(2, "mini: export: `%s': not a valid identifier\n",
				str), 0);
	pos = char_index(str, '=');
	if (pos > 0 && str[pos - 1] == '+')
		return (validate_append_identifier(str, pos));
	return (validate_regular_identifier(str));
}
