/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset_b.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:33:46 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 04:53:04 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/*Unset with no arugment do nothing
loops through argument.
Append '=' to the name of a variable if not present.
VAR is replace with VAR=
if found use array_replace to shift everything*/
int    builtin_unset_b(t_cmd_set *p, char **args)
{
    int        i;
    char    *tmpstr;
    int        var_index;

    if (ft_arr_len(args) <= 1)
        return (0);
    i = 1;
    while (args[i])
    {
        if (args[i][0] == '-' && ft_strcmp(args[i], "--") != 0)
        {
            ft_printf_fd(2, "mini: unset: %s': invalid option\n", args[i]);
            return (2);
        }
        i++;
    }
    i = 1;
    while (args[i])
    {
        if (ft_strcmp(args[i], "--") == 0)
        {
            i++;
            continue;
        }

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
        i++;
    }
    return (0);
}
