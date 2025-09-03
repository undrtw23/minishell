/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:33:46 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:47:50 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*Unset with no arugment do nothing
loops through argument.
Append '=' to the name of a variable if not present.
VAR is replace with VAR=
// if found use array_replace to shift everything*/

int	has_invalid_options(char **args)
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (args[i][0] == '-' && ft_strcmp(args[i], "--") != 0)
		{
			ft_printf_fd(2, "mini: unset: %s': invalid option\n", args[i]);
			return (1);
		}
		i++;
	}
	return (0);
}

void	ensure_equals_suffix(char **arg)
{
	int		len;
	char	*tmp;

	len = ft_strlen(*arg);
	if (len > 0 && (*arg)[len - 1] != '=')
	{
		tmp = ft_strjoin(*arg, "=");
		free(*arg);
		*arg = tmp;
	}
}

void	remove_env_var(char *arg, char ***envp)
{
	int	var_index;

	var_index = find_env_var_index(arg, *envp);
	if (var_index != -1)
		ft_array_replace(envp, NULL, var_index);
}

int	builtin_unset_b(t_cmd_set *p, char **args)
{
	int	i;

	if (ft_arr_len(args) <= 1)
		return (0);
	if (has_invalid_options(args))
		return (2);
	i = 1;
	while (args[i])
	{
		if (ft_strcmp(args[i], "--") == 0)
		{
			i++;
			continue ;
		}
		ensure_equals_suffix(&args[i]);
		remove_env_var(args[i], &p->envp);
		i++;
	}
	return (0);
}
