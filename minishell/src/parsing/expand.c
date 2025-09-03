/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:30:48 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 21:36:51 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*handle_j_equal_one(char *left, char *home, char *right)
{
	char	*expanded;
	char	*home_no_slash;

	if (home[ft_strlen(home) - 1] == '/')
	{
		home_no_slash = ft_substr(home, 0, ft_strlen(home) - 1);
		expanded = ft_strjoin3(left, home_no_slash, right);
		free(home_no_slash);
	}
	else
		expanded = ft_strjoin3(left, home, right);
	return (expanded);
}

char	*handle_j_greater_than_one(char *left, char *home, char *right)
{
	char	*expanded;
	char	*home_with_slash;

	if (home[ft_strlen(home) - 1] != '/')
	{
		home_with_slash = ft_strjoin(home, "/");
		expanded = ft_strjoin3(left, home_with_slash, right);
		free(home_with_slash);
	}
	else
		expanded = ft_strjoin3(left, home, right);
	return (expanded);
}

char	*build_expanded_string(char *left, char *home, char *right, int j)
{
	char	*expanded;

	if (j == 1)
		expanded = handle_j_equal_one(left, home, right);
	else
		expanded = handle_j_greater_than_one(left, home, right);
	return (expanded);
}

char	*expand_tilde_at_position(char *str, int i, t_cmd_set *p, char *s[3])
{
	char	*home;
	char	*expanded;
	char	*left;
	char	*right;
	int		j;

	(void)s;
	j = get_tilde_expansion_length(str, i);
	if (j == 0)
		return (str);
	home = get_home_dir(p);
	left = ft_substr(str, 0, i);
	right = ft_strdup(&str[i + j]);
	expanded = build_expanded_string(left, home, right, j);
	free_all(home, left, right, str);
	return (expanded);
}

/*	expand tilde at position end*/

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
	char	*expanded;

	expanded = NULL;
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
			expanded = expand_tilde_at_position(str, i, p, s);
			if (expanded != str)
				return (var_expander(expanded, quotes, p));
		}
	}
	return (str);
}
