/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environement.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/17 16:31:45 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 20:16:02 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Find a specific environnement variable var in envp
iterate through whole env and return the index of the var
if found or -1 if nothing found.*/
int	find_env_var_index(char *var, char **envp)
{
	int	i;
	int	n;
	int	k;

	i = 0;
	n = ft_strlen(var);
	while (envp && envp[i])
	{
		k = n;
		if (k < char_index(envp[i], '='))
			k = char_index(envp[i], '=');
		if (!ft_strncmp(envp[i], var, k))
		{
			return (i);
		}
		i++;
	}
	return (-1);
}

/* Get a specific environnement value.
if var is empty return '$' (empty var)
if found find_env go through var till '=' found (if not reutnr NULL)
increment one time so it point to the first char after '='
return a duplicate of it*/
char	*ft_getenv(char *var, char **envp)
{
	int			var_index;
	const char	*env_var;
	const char	*value_start;

	if (var[0] == 0)
		return (ft_strdup("'$'"));
	var_index = find_env_var_index(var, envp);
	if (var_index == -1)
		return (NULL);
	env_var = envp[var_index];
	value_start = ft_strchr(env_var, '=');
	if (value_start)
	{
		value_start++;
		return (ft_strdup(value_start));
	}
	return (NULL);
}

/* Set or update an environement variable.
if it exist, replace it with a new value.
if not create the new variable and insert it via ft_array_insert*/
char	**ft_setenv(char *var, char *value, char **envp)
{
	int		index;
	char	*tmpstr[2];

	index = find_env_var_index(var, envp);
	if (index != -1)
	{
		tmpstr[0] = ft_strjoin(var, "=");
		if (tmpstr[0])
		{
			free(envp[index]);
			envp[index] = ft_strjoin(tmpstr[0], value);
			free(tmpstr[0]);
		}
	}
	else
	{
		tmpstr[0] = ft_strjoin(var, "=");
		if (tmpstr[0])
		{
			tmpstr[1] = ft_strjoin(tmpstr[0], value);
			envp = ft_array_insert(envp, tmpstr[1]);
			free_all(tmpstr[0], tmpstr[1], NULL, NULL);
		}
	}
	return (envp);
}
/* Helper for expanding environement variable in Heredoc.*/

void	handle_env_vars(char *str, int *i, int fd[2], t_cmd_set *p)
{
	int		j;
	char	*tmp;
	char	*tmp2;

	j = *i + 1;
	while (str[j])
	{
		if (!ft_isalnum(str[j]) || str[j] == ' ' || str[j] == '\n'
			|| str[j] == '\t' || str[j] == '$' || str[j] == '\"'
			|| str[j] == '\'' || str[j] == '<')
			break ;
		j++;
	}
	tmp = ft_substr(str, *i + 1, j - *i - 1);
	tmp2 = ft_getenv(tmp, p->envp);
	if (tmp2)
		write(fd[1], tmp2, ft_strlen(tmp2));
	*i = j - 1;
	free_all(tmp, tmp2, NULL, NULL);
}
