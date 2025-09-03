/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:39:27 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:56:11 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_heredoc_success(t_cmd_set *p, int syntax)
{
	if (!p->tokens)
	{
		free_array(&p->tokens);
		return (1);
	}
	if (syntax != 999)
	{
		free_array(&p->tokens);
		return (2);
	}
	p = parse_nodes(p->tokens, p);
	free_array(&p->tokens);
	return (0);
}

void	cleanup_process(t_cmd_set *p)
{
	if (p->input_text)
		free_array(&p->input_text);
	if (p->filename)
		free_filename_tab(p);
	close_fds_except_std();
}

int	process_input_helper(t_cmd_set *p, int *syntax)
{
	int	result;

	result = handle_heredoc_success(p, *syntax);
	if (result == 1)
		return (1);
	else if (result == 2)
		return (2);
	return (0);
}

void	*process_input(char **input, t_cmd_set *p)
{
	int	line_index;
	int	len_input_arr;
	int	syntax;

	if (!init_process_vars(input, &len_input_arr, &line_index, &syntax))
		return (NULL);
	while (line_index < len_input_arr)
	{
		if (init_line_processing(input, p, line_index, &syntax)
			&& handle_uquote(&input[line_index], p))
			return (p);
		p->tokens = tokenize_input(&input[line_index], p);
		if (!(p->tokens))
			break ;
		process_syntax_and_tokens(p, &syntax, &line_index);
		if (read_heredocs(p, &line_index, syntax) != syntax && p->tokens)
			free_array(&p->tokens);
		else
		{
			if (process_input_helper(p, &syntax) == 1)
				return (p);
			continue ;
		}
	}
	return (cleanup_process(p), p);
}
