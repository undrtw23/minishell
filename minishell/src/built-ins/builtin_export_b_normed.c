/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_b_normed.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 22:46:47 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 02:53:07 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Print all variable in the model :
declare -x VAR="value".
declare -x VAR (if no =)
Skip _= variable.*/
void	print_export(char **envp)
{
	int		i;
	int		pos;
	char	*key;

	sort_env(envp, ft_arr_len(envp));
	i = 0;
	while (envp && envp[i])
	{
		pos = char_index(envp[i], '=');
		if (pos == -1)
			key = ft_strdup(envp[i]);
		else
			key = ft_substr(envp[i], 0, pos);
		if (key && ft_strcmp(key, "_"))
		{
			if (pos == -1)
				ft_printf_fd(1, "declare -x %s\n", key);
			else
				ft_printf_fd(1, "declare -x %s=\"%s\"\n",
					key, envp[i] + pos + 1);
		}
		free(key);
		i++;
	}
}

/* If no argument, print_export
Loops through all aguments :
Check for valid identifier
check if already exit via var_in_envp
if found duplicate and store it at the same place (free before)
if not found, append it at the end via ft_array_insert.*/

void	handle_export_var(t_cmd_set *p, char *arg, int *status)
{
	int	pos;
	int	idx;

	if (!is_valid_identifier(arg))
	{
		*status = 1;
		return ;
	}
	pos = var_in_envp(arg, p->envp, &idx);
	if (pos == 1)
	{
		free(p->envp[idx]);
		p->envp[idx] = ft_strdup(arg);
	}
	else if (pos == 0)
		p->envp = ft_array_insert(p->envp, arg);
}

int	builtin_export_b(t_cmd_set *p, char **args)
{
	int	i;
	int	status;

	p->status_code = 0;
	status = 0;
	if (!args[1])
		return (print_export(p->envp), 0);
	i = 0;
	while (args[++i])
		handle_export_var(p, args[i], &status);
	return (status);
}
