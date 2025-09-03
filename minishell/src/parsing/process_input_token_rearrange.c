/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_token_rearrange.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:03:42 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:07:54 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_valid_join_condition(char ***a, char ***filename, int i,
		int token_counter)
{
	return ((*filename)[i] != NULL && i > 0 && get_precedence((*a)[i - 1]) < 2
		&& (i + 1) < token_counter && get_precedence((*a)[i + 1]) < 2);
}

void	handle_join_case(char ***a, int i)
{
	char	*tmp[2];

	tmp[0] = ft_strjoin((*a)[i - 1], " ");
	tmp[1] = ft_strjoin(tmp[0], (*a)[i + 1]);
	free_all((*a)[i - 1], (*a)[i + 1], tmp[0], NULL);
	(*a)[i - 1] = tmp[1];
	(*a)[i + 1] = NULL;
}

int	is_valid_swap_condition(char ***a, char ***filename, int i,
		int token_counter)
{
	return ((*filename)[i] != NULL && (i + 1) < token_counter
		&& get_precedence((*a)[i + 1]) < 2);
}

void	handle_swap_case(char ***a, char ***filename, int i)
{
	char	*tmp;

	tmp = (*a)[i + 1];
	(*a)[i + 1] = (*a)[i];
	(*a)[i] = tmp;
	(*filename)[i + 1] = (*filename)[i];
	(*filename)[i] = NULL;
}

void	process_token(char ***a, char ***filename, int i, int token_counter)
{
	if ((*a)[i] == NULL)
		return ;
	if (get_precedence((*a)[i]) == 4 && (*filename)[i])
	{
		if (is_valid_join_condition(a, filename, i, token_counter))
		{
			handle_join_case(a, i);
		}
		else if (is_valid_swap_condition(a, filename, i, token_counter))
		{
			handle_swap_case(a, filename, i);
		}
	}
}
