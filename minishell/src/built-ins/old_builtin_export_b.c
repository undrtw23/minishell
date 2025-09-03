/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export_b.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 15:35:26 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/02 22:46:23 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Check for a viable of name arg, update the index of the var if found
if found and has a value (= sign) return 1
-1 if no value
0 if not found.*/
static int	var_in_envp(char *arg, char **envp, int *idx)
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
static int	is_valid_identifier(char *str)
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
static void	sort_env(char **arr, int len)
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

/* Print all variable in the model :
declare -x VAR="value".
declare -x VAR (if no =)
Skip _= variable.*/
static void	print_export(char **envp)
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
int	builtin_export_b(t_cmd_set *p, char **args)
{
	int	i;
	int	pos;
	int	idx;
	int	status;

	p->status_code = 0;
	status = 0;
	if (!args[1])
		return (print_export(p->envp), 0);
	i = 0;
	while (args[++i])
	{
		if(!is_valid_identifier(args[i]))
		{
			status = 1;
			continue;
		}
		pos = var_in_envp(args[i], p->envp, &idx);
		if (pos == 1)
		{
			free(p->envp[idx]);
			p->envp[idx] = ft_strdup(args[i]);
		}
		else if (pos == 0)
			p->envp = ft_array_insert(p->envp, args[i]);
	}
	return (status);
}


