/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helper_2.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:36:26 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:15:37 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Find both exit status for $? or variable value ($USER, $HOME....)
if variable doesnt exist, return ""*/
char	*find_substitution(char first, char *var, t_cmd_set *p)
{
	char	*val;

	val = NULL;
	if (first == '?')
	{
		if (g_exit_status > 0)
		{
			p->status_code = g_exit_status;
			g_exit_status = 0;
		}
		val = ft_itoa(p->status_code);
	}
	else if (var)
		val = ft_getenv(var, p->envp);
	free(var);
	if (!val)
		val = ft_strdup("");
	return (val);
}

/* Actual expamder, first check the lenght of the variable to send
a copie to "find_substitution" that will replace it with its actual value
i.e. exit code if '?' or var value for '$'.
Reconstruct the string via append left and right to the variable value
call var_expander on the result with quoting value are reset to 0.
Recursion stop when "else if" not entered.*/
char	*expand_variable(char *str, int i, t_cmd_set *p, char *s[3])
{
	int		j;
	char	*res;
	char	*left;
	char	*right;

	j = 0;
	if (str[i] == '?')
		j = 1;
	else
		while (str[i + j] && (ft_isalnum(str[i + j]) || str[i + j] == '_'))
			j++;
	s[0] = ft_substr(str, i, j);
	s[1] = find_substitution(str[i], s[0], p);
	left = ft_substr(str, 0, i - 1);
	right = ft_strdup(&str[i + j]);
	s[2] = ft_strjoin(left, s[1]);
	res = ft_strjoin(s[2], right);
	free_all(left, right, s[1], s[2]);
	free(str);
	return (res);
}

char	*expand_tilde(char *str, t_cmd_set *p)
{
	char	*home;
	char	*expanded;
	char	*remaining;
	int		i;

	if (!str || str[0] != '~')
		return (str);
	home = ft_getenv("HOME", p->envp);
	if (!home)
		home = ft_strdup("/");
	i = 1;
	if (str[i] == '/')
		i++;
	else if (ft_isalnum(str[i]) || str[i] == '_')
	{
		while (str[i] && (ft_isalnum(str[i]) || str[i] == '_' || str[i] == '-'))
			i++;
		if (str[i] == '/')
			i++;
	}
	remaining = ft_strdup(str + i);
	expanded = ft_strjoin(home, remaining);
	free_all(home, remaining, str, NULL);
	return (expanded);
}

int	get_tilde_expansion_length(char *str, int i)
{
	int	j;

	j = 1;
	if (str[i + j] != '/' && str[i + j] != '\0' && str[i + j] != ' ' && str[i
			+ j] != '\t')
		return (0);
	if (str[i + j] == '/')
		j++;
	return (j);
}

char	*get_home_dir(t_cmd_set *p)
{
	char	*home;

	home = ft_getenv("HOME", p->envp);
	if (!home)
		home = ft_strdup("/");
	return (home);
}
