/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwang <cwang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:33:46 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/28 16:23:17 by cwang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*Unset with no arugment do nothing
loops through argument.
Append '=' to the name of a variable if not present.
VAR is replace with VAR=
if found use array_replace to shift everything*/
int	builtin_unset(t_cmd_set *p, char **args)
{
	int		i;
	char	*tmpstr;
	int		var_index;

	i = -1;
	if (ft_arr_len(args) >= 2)
	{
		while (args[++i])
		{
			if (ft_strlen(args[i]) && args[i][ft_strlen(args[i]) - 1]
				&& args[i][ft_strlen(args[i]) - 1] != '=')
			{
				tmpstr = ft_strjoin(args[i], "=");
				free(args[i]);
				args[i] = tmpstr;
			}
			var_index = find_env_var_index(args[i], p->envp);
			if (var_index != -1)
				ft_array_replace(&p->envp, NULL, var_index);
		}
	}
	return (0);
}
