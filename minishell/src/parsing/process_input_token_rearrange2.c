/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_token_rearrange2.c                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:03:45 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:08:31 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	rearrange_tab(char ***a, char ***filename, int token_counter)
{
	int	i;

	i = -1;
	while (++i < token_counter)
	{
		process_token(a, filename, i, token_counter);
	}
}

void	count_valid_entries(char ***a, char ***filename, int token_count,
		int *counts)
{
	int	i;

	i = -1;
	counts[0] = 0;
	counts[1] = 0;
	while (++i < token_count)
	{
		if ((*a)[i])
		{
			counts[0]++;
			if ((*filename)[i] != NULL)
				counts[1]++;
		}
	}
	counts[2] = counts[0] + counts[1];
}

int	allocate_new_tab(char ***newtab, int total_size)
{
	*newtab = (char **)malloc((total_size + 1) * sizeof(char *));
	if (!*newtab)
		return (0);
	(*newtab)[total_size] = NULL;
	return (1);
}

void	copy_and_cleanup_entries(char ***a, char ***filename, char **newtab,
		int token_count)
{
	int	i;
	int	new_index;

	i = -1;
	new_index = 0;
	while (++i < token_count)
	{
		if ((*a)[i])
		{
			newtab[new_index] = ft_strdup((*a)[i]);
			new_index++;
			if ((*filename)[i])
			{
				newtab[new_index] = ft_strdup((*filename)[i]);
				new_index++;
			}
			free_all((*a)[i], (*filename)[i], NULL, NULL);
			(*filename)[i] = NULL;
		}
	}
}

char	**realloc_tab(char ***a, char ***filename, int token_count)
{
	char	**newtab;
	int		counts[3];

	newtab = NULL;
	count_valid_entries(a, filename, token_count, counts);
	if (!allocate_new_tab(&newtab, counts[2]))
		return (NULL);
	copy_and_cleanup_entries(a, filename, newtab, token_count);
	free(*a);
	return (newtab);
}
