/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_b2_normed.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 22:46:47 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 02:56:06 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Check for a viable of name arg, update the index of the var if found
if found and has a value (= sign) return 1
-1 if no value
0 if not found.*/
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

/* Check for the name of the variable :
invalid :
Empty string.
Starts with digit or =.
Any character before = is not alphanumeric or underscore.*/
int	is_valid_identifier(char *str)
{
	int	pos;

	if (!str || str[0] == '\0')
		return (ft_printf_fd(2,
				"mini: export: `%s': not a valid identifier\n",
				str), 0);
	if (ft_isdigit(str[0]) || str[0] == '=')
		return (ft_printf_fd(2,
				"mini: export: `%s': not a valid identifier\n",
				str), 0);
	pos = 0;
	while (str[pos] && str[pos] != '=')
	{
		if (!ft_isalnum(str[pos]) && str[pos] != '_')
			return (ft_printf_fd(2,
					"mini: export: `%s': not a valid identifier\n", str), 0);
		pos++;
	}
	return (1);
}

/* Sorts the environment variable array alphabetically, 
so export prints in order. 
Bubble sorting FTW !*/
void	sort_env(char **arr, int len)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (i < len - 1)
	{
		j = 0;
		while (j < len - i - 1)
		{
			if (ft_strcmp(arr[j], arr[j + 1]) > 0)
			{
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}
