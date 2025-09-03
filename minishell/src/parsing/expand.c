/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:30:48 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 02:22:58 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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
	
	free(home);
	free(remaining);
	free(str);
	return (expanded);
}

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*tmp;
	char	*result;
	
	tmp = ft_strjoin(s1, s2);
	result = ft_strjoin(tmp, s3);
	free(tmp);
	return (result);
}

char	*expand_tilde_at_position(char *str, int i, t_cmd_set *p, char *s[3])
{
	char	*home;
	char	*expanded;
	char	*left;
	char	*right;
	int		j;

	(void)s;
	j = 1;
	
	if (str[i + j] != '/' && str[i + j] != '\0' && 
		str[i + j] != ' ' && str[i + j] != '\t')
		return (str);
	
	if (str[i + j] == '/')
		j++;
	
	home = ft_getenv("HOME", p->envp);
	if (!home)
		home = ft_strdup("/");
	
	left = ft_substr(str, 0, i);
	right = ft_strdup(&str[i + j]);
	
	if (j == 1)
	{
		if (home[ft_strlen(home) - 1] == '/')
		{
			char *home_no_slash = ft_substr(home, 0, ft_strlen(home) - 1);
			expanded = ft_strjoin3(left, home_no_slash, right);
			free(home_no_slash);
		}
		else
			expanded = ft_strjoin3(left, home, right);
	}
	else
	{
		if (home[ft_strlen(home) - 1] != '/')
		{
			char *home_with_slash = ft_strjoin(home, "/");
			expanded = ft_strjoin3(left, home_with_slash, right);
			free(home_with_slash);
		}
		else
			expanded = ft_strjoin3(left, home, right);
	}
	
	free(home);
	free(left);
	free(right);
	free(str);
	return (expanded);
}

/* Main recursive loop for expanding var, first remove useless $ before quotes
quotes[0] : inside single quote
quotes[1] : inside double quote
tilde_expander for HOME (only if not in single/double quotes)
then if it finds $ that is followed by a valid var, the recursion begin
until there is nor more variable to expand.*/
char	*var_expander(char *str, int *quotes, t_cmd_set *p)
{
	int		i;
	char	*s[3];

	quotes[0] = 0;
	quotes[1] = 0;
	i = -1;

	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		
		if (!quotes[0] && str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
			return (var_expander(expand_variable(str, ++i, p, s), quotes, p));
		
		if (!quotes[0] && !quotes[1] && str[i] == '~')
		{
			char *expanded = expand_tilde_at_position(str, i, p, s);
			if (expanded != str)
				return (var_expander(expanded, quotes, p));
		}
	}
	return (str);
}