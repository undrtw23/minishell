/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:30:48 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/08/27 17:56:46 by alsima           ###   ########.fr       */
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
	// ft_printf_fd(2, "varexp str=%s\n", str);
	while (str && str[++i])
	{
		quotes[0] = (quotes[0] + (!quotes[1] && str[i] == '\'')) % 2;
		quotes[1] = (quotes[1] + (!quotes[0] && str[i] == '\"')) % 2;
		// ft_printf_fd(2, "	varexp q[0]=%i str + %i = %s\n", quotes[0], i, str + i);
		if (!quotes[0] && str[i] == '$' && str[i + 1] && (ft_isalpha(str[i + 1])
				|| str[i + 1] == '_' || str[i + 1] == '?'))
			return (var_expander(expand_variable(str, ++i, p, s), quotes, p));
	}
	return (str);
}
