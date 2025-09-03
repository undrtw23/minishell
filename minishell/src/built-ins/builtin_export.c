/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 03:07:51 by alsima            #+#    #+#             */
/*   Updated: 2025/09/03 20:02:52 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
				ft_printf_fd(1, "declare -x %s=\"%s\"\n", key, envp[i] + pos
					+ 1);
		}
		free(key);
		i++;
	}
}

void	handle_append_operation(char *arg, t_cmd_set *p)
{
	char	*equals;
	char	*var_name;
	char	*old_value;
	char	*new_value;

	equals = ft_strchr(arg, '=');
	var_name = ft_substr(arg, 0, equals - arg - 1);
	old_value = ft_getenv(var_name, p->envp);
	if (old_value)
		new_value = ft_strjoin(old_value, equals + 1);
	else
		new_value = ft_strdup(equals + 1);
	p->envp = ft_setenv(var_name, new_value, p->envp);
	free(var_name);
	free(old_value);
	free(new_value);
}

void	process_export_arg(char *arg, t_cmd_set *p, int *status)
{
	char	*equals;
	int		idx;
	int		pos;

	equals = ft_strchr(arg, '=');
	if (!is_valid_identifier(arg))
		*status = 1;
	else if ((equals && equals > arg && *(equals - 1) == '+'))
		handle_append_operation(arg, p);
	else
	{
		pos = var_in_envp(arg, p->envp, &idx);
		if (pos == 1)
		{
			free(p->envp[idx]);
			p->envp[idx] = ft_strdup(arg);
		}
		else if (pos == 0)
			p->envp = ft_array_insert(p->envp, arg);
	}
}

int	builtin_export_b(t_cmd_set *p, char **args)
{
	int	i;
	int	has_flag_error;
	int	status;

	has_flag_error = 0;
	p->status_code = 0;
	status = 0;
	if (!args[1])
		return (print_export(p->envp), 0);
	i = 0;
	while (args[++i])
	{
		if (args[i][0] == '-' && !((ft_strcmp(args[i], "--") == 0
					|| ft_strcmp(args[i], "-n") == 0 || ft_strcmp(args[i],
						"-f") == 0)))
		{
			ft_printf_fd(2, "mini: export: `%s': invalid option\n", args[i]);
			has_flag_error = 1;
			continue ;
		}
		process_export_arg(args[i], p, &status);
	}
	if (has_flag_error)
		return (2);
	return (status);
}
