/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trim_quotes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cwang <cwang@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:40:47 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/05/28 16:24:53 by cwang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* As always, inside quotes checker.
Calculate exactly of many spaces need to be removed for the new malloc.
Check if quotes are not closed (security as it's already done before)*/
static int	check_quotes_get_len(char const *s1)
{
	int	count;
	int	i;
	int	dquote;
	int	squote;

	i = 0;
	count = 0;
	dquote = 0;
	squote = 0;
	while (s1 && s1[i])
	{
		squote = (squote + (!dquote && s1[i] == '\'')) % 2;
		dquote = (dquote + (!squote && s1[i] == '\"')) % 2;
		if ((s1[i] == '\"' && !squote) || (s1[i] == '\'' && !dquote))
			count++;
		i++;
	}
	if (squote || dquote)
		return (-1);
	return (count);
}

/* If the character is not a quote, or it's a quote inside the other kind
(i.e  single quote inside "double"') -> keep it.
Otherwise (it's an outer quote character), skip it*/
char	*remove_quotes(char const *s1, int squote, int dquote)
{
	int		i;
	int		j;
	char	*str;

	i = -1;
	j = -1;
	if (!s1 || check_quotes_get_len(s1) == -1)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) - check_quotes_get_len(s1) + 1));
	if (!str)
		return (NULL);
	while (s1[++j])
	{
		squote = (squote + (!dquote && s1[j] == '\'')) % 2;
		dquote = (dquote + (!squote && s1[j] == '\"')) % 2;
		if ((s1[j] != '\"' || squote) && (s1[j] != '\'' || dquote) && ++i >= 0)
			str[i] = s1[j];
	}
	str[++i] = '\0';
	return (str);
}
