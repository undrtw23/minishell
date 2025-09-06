/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_token_count2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:03:37 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/06 16:32:25 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	count_tokens(char *input, char *ops)
{
	int	q[2];
	int	count;

	count = 0;
	q[0] = 0;
	q[1] = 0;
	while (*input)
	{
		handle_whitespace(&input, q);
		if (!*input)
			break ;
		if (!is_operator_char(*input, ops))
		{
			handle_non_operator(&input, q, &count, ops);
		}
		else
		{
			handle_operator(&input, &count);
		}
	}
	return (count);
}

char	*malloc_and_copy(char *index, int l)
{
	char	*str;
	int		i;

	i = l;
	while (i > 0 && (index[i - 1] == ' ' || index[i - 1] == '\t'))
		i--;
	str = ft_calloc(sizeof(char), i + 1);
	if (!str)
		return (NULL);
	l = -1;
	while (++l < i)
		str[l] = index[l];
	str[l] = 0;
	return (str);
}

char	*fill_token_tab_word(char *input, char *ops, int *q)
{
	char	*index;

	index = input;
	while (*input && (is_in_quote(*input, q) || !ft_strchr(ops, *input)))
		input++;
	return (malloc_and_copy(index, (int)(input - index)));
}

void	handle_word_token(char **input, t_token_params *params)
{
	(*params->a)[*params->i] = fill_token_tab_word(*input, params->ops,
			params->q);
	*input += ft_strlen((*params->a)[*params->i]);
	(*params->i)++;
}

void	handle_operator_token(char **input, char ***a, int *i)
{
	if (**input && *(*input + 1) && *(*input + 2) && **input == '<' && *(*input
			+ 1) == '<' && *(*input + 2) == '<')
	{
		(*a)[*i] = malloc_and_copy(*input, 3);
	}
	else if ((**input && *(*input + 1) && **input == *(*input + 1)
			&& (**input == '<' || **input == '>' || **input == '|'
				|| **input == '&')) || (**input == '<' && *(*input + 1) == '>'))
	{
		(*a)[*i] = malloc_and_copy(*input, 2);
	}
	else if (**input && *(*input + 1) && **input == '>' && *(*input + 1) == '|')
	{
		(*a)[*i] = malloc_and_copy((*input)++, 1);
	}
	else
		(*a)[*i] = malloc_and_copy(*input, 1);
	*input += ft_strlen((*a)[*i]);
	(*i)++;
}
