/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_token_count.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:03:34 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:05:36 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	handle_whitespace(char **input, int *q)
{
	while (**input && is_whitespace_in_quote(**input, q))
		(*input)++;
}

void	handle_non_operator(char **input, int *q, int *count, char *ops)
{
	while (**input && (is_in_quote(**input, q) || !is_operator_char(**input,
				ops)))
		(*input)++;
	(*count)++;
}

int	is_triple_redirect(char *input)
{
	return (input[0] && input[1] && input[2] && input[0] == '<'
		&& input[1] == '<' && input[2] == '<');
}

int	is_double_operator(char *input)
{
	if (!input[0] || !input[1])
		return (0);
	if ((input[0] == input[1] && (input[0] == '<' || input[0] == '>'
				|| input[0] == '|' || input[0] == '&')) || (input[0] == '<'
			&& input[1] == '>'))
	{
		return (1);
	}
	return (0);
}

void	handle_operator(char **input, int *count)
{
	if (is_triple_redirect(*input))
	{
		*input += 2;
	}
	else if (is_double_operator(*input))
	{
		(*input)++;
	}
	(*count)++;
	(*input)++;
}
