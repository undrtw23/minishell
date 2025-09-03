/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input_helper2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 22:03:17 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 22:10:35 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_filename_tab(t_cmd_set *p)
{
	int	i;

	i = -1;
	while (++i < 4096)
		if (p->filename[i])
			free(p->filename[i]);
}

int	init_process_vars(char **input, int *len_input_arr, int *line_index,
		int *syntax)
{
	*syntax = 999;
	*len_input_arr = ft_arr_len(input);
	*line_index = 0;
	if (!input)
		return (0);
	return (1);
}

int	init_line_processing(char **input, t_cmd_set *p, int line_index,
		int *syntax)
{
	*syntax = 999;
	p->token_count = 0;
	p->abort = 0;
	init_heredoc_arr(p);
	if (input[line_index][0] != '\0')
		add_history(input[line_index]);
	return (check_unclosed_quotes(input[line_index]));
}

char	**tokenize_input(char **input_line, t_cmd_set *p)
{
	handle_input(input_line, p);
	return (split_by_op_ign_space_in_quote(*input_line, "<|>&()", p));
}

void	process_syntax_and_tokens(t_cmd_set *p, int *syntax,
		int *line_index)
{
	if (p->tokens && *p->tokens[0])
		*syntax = check_syntax(p->tokens, p->token_count, p);
	fill_filename_tab(&p->tokens, p->token_count, p);
	rearrange_tab(&p->tokens, &p->filename, p->token_count);
	p->tokens = realloc_tab(&p->tokens, &p->filename, p->token_count);
	p->token_count = ft_arr_len(p->tokens);
	(*line_index)++;
}
