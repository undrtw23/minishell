/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_token_fill.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:03:40 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:07:12 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	fill_token_tab(char *input, char ***a, char *ops, int k)
{
	int				i;
	int				q[2];
	t_token_params	params;

	q[0] = 0;
	q[1] = 0;
	i = 0;
	params.a = a;
	params.i = &i;
	params.ops = ops;
	params.q = q;
	while (i < k)
	{
		if ((*input == ' ' || *input == '\t') && !q[0] && !q[1])
			input++;
		else if (!ft_strchr(ops, *input))
			handle_word_token(&input, &params);
		else
			handle_operator_token(&input, a, &i);
	}
	(*a)[i] = 0;
}

char	**init_filename_tab(void)
{
	int		i;
	char	**filename;

	filename = (char **)malloc(sizeof(char *) * 4096);
	if (!filename)
		return (NULL);
	i = -1;
	while (++i < 4096)
		filename[i] = NULL;
	return (filename);
}

char	*strchr_outside_quotes(char *s, char c)
{
	int	q[2];
	int	i;

	q[0] = 0;
	q[1] = 0;
	i = 0;
	if ((char)c == '\0')
		return ((char *)&s[ft_strlen(s)]);
	while (s[i] != '\0')
	{
		if (s[i] == '\'' && !q[1])
			q[0] = !q[0];
		else if (s[i] == '\"' && !q[0])
			q[1] = !q[1];
		if ((char)c == s[i] && !q[0] && !q[1])
			return ((char *)&s[i]);
		i++;
	}
	return (NULL);
}

void	fill_filename_tab(char ***a, int token_count, t_cmd_set *p)
{
	char	*aux[2];
	int		i;

	i = -1;
	if (!p->filename)
		p->filename = init_filename_tab();
	while (++i < token_count)
	{
		if (get_precedence((*a)[i]) == 4 && (i + 1) < token_count
			&& get_precedence((*a)[i + 1]) < 2)
		{
			aux[0] = strchr_outside_quotes((*a)[i + 1], ' ');
			if (aux[0])
			{
				aux[1] = ft_strtrim(aux[0], " \t");
				p->filename[i] = ft_substr((*a)[i + 1], 0, aux[0] - (*a)[i
						+ 1]);
				free((*a)[i + 1]);
				(*a)[i + 1] = aux[1];
			}
		}
	}
}
