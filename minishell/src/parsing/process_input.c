/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_input.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:39:27 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/02 22:16:21 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Checks if a string has unclosed single or double quotes
that are not respectivly themselves into double/single quotes
Returns 1 if unclosed, 0 otherwise. */

/*UPD Checks also for unclosed \ and removes them*/
int	check_unclosed_quotes(const char *s)
{
	int	squote;
	int	dquote;
	int	i;

	squote = 0;
	dquote = 0;
	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' && dquote == 0)
			squote = !squote;
		else if (s[i] == '\"' && squote == 0)
			dquote = !dquote;
		i++;
	}
	return (squote || dquote);
}

/* Helper for freeing input whn encounter unclosed quotes. */
static void	handle_uquote(char **s, t_cmd_set *p)
{
	(void)s;
	put_err("Unclosed_Quote", NULL, 2, p);
	//	free_all(*s, NULL, NULL, NULL);
}

int	is_in_quote(char c, int (*q)[2])
{
	(*q)[0] = ((*q)[0] + (!(*q)[1] && c == '\'')) % 2;
	(*q)[1] = ((*q)[1] + (!(*q)[0] && c == '\"')) % 2;
	if ((*q)[0] || (*q)[1])
		return (1);
	else
		return (0);
}

// counts tokens; 3 types
//	ops: < > | << >> || && <>
//	non-ops: anything between 2 ops with trimmed space at beginning and end
int	count_tokens(char *input, char *ops)
{
	int	q[2];
	int	k;

	k = 0;
	q[0] = 0;
	q[1] = 0;
	while (*input)
	{
		if ((*input == ' ' || *input == '\t') && !q[0] && !q[1])
			input++;
		else if (!ft_strchr(ops, *input))
		{
			while (*input && (is_in_quote(*input, &q) || !ft_strchr(ops,
						*input)))
				input++;
			k++;
		}
		else
		{
			if (*input && *(input + 1) && *(input + 2) && *input == '<'
				&& *(input + 1) == '<' && *(input + 2) == '<')
				input += 2;
			else if ((*input && *(input + 1) && *input == *(input + 1)
					&& (*input == '<' || *input == '>' || *input == '|'
						|| *input == '&')) || (*input == '<' && *(input
						+ 1) == '>'))
				input++;
			k++;
			input++;
		}
	}
	return (k);
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

char	*fill_token_tab_word(char *input, char *ops, int (*q)[2])
{
	char	*index;

	index = input;
	while (*input && (is_in_quote(*input, q) || !ft_strchr(ops, *input)))
		input++;
	return (malloc_and_copy(index, (int)(input - index)));
}
void	fill_token_tab(char *input, char ***a, char *ops, int k)
{
	int	i;
	int	q[2];

	q[0] = 0;
	q[1] = 0;
	i = 0;
	while (i < k)
	{
		if ((*input == ' ' || *input == '\t') && !q[0] && !q[1])
			input++;
		else if (!ft_strchr(ops, *input))
		{
			(*a)[i] = fill_token_tab_word(input, ops, &q);
			input += ft_strlen((*a)[i]);
			i++;
		}
		else
		{
			if (*input && *(input + 1) && *(input + 2) && *input == '<'
				&& *(input + 1) == '<' && *(input + 2) == '<')
				(*a)[i] = malloc_and_copy(input, 3);
			else if ((*input && *(input + 1) && *input == *(input + 1)
					&& (*input == '<' || *input == '>' || *input == '|'
						|| *input == '&')) || (*input == '<' && *(input
						+ 1) == '>'))
				(*a)[i] = malloc_and_copy(input, 2);
			else if (*input && *(input + 1) && *input == '>' && *(input
					+ 1) == '|')
				(*a)[i] = malloc_and_copy(input++, 1);
			else
				(*a)[i] = malloc_and_copy(input, 1);
			input += ft_strlen((*a)[i]);
			i++;
		}
	}
	(*a)[i] = 0;
}

char	**init_filename_tab(void)
{
	int		i;
	char	**filename;

	filename = (char **)malloc(sizeof(char *) * 1024);
	if (!filename)
		return (NULL);
	i = -1;
	while (++i < 1024)
		filename[i] = NULL;
	return (filename);
}
// modified strchr return pointer to char c if it's not contained in quotes

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
	// ft_printf_fd(1, "IN FILL_FILENAME_TAB\n");
	while (++i < token_count)
	{
		// if (get_precedence((*a)[i]) == 4 && (i + 1) < token_count
		// 	&& get_precedence((*a)[i + 1]) == 4)
		// {
		// 	put_err("Unexpected_Token", (*a)[i + 1], 2, p);
		// 	if (get_nodetype((*a)[i]) == N_HEREDOC)
		// 		*syntax = 3;
		// 	else
		// 		*syntax = 2;
		// }
		if (get_precedence((*a)[i]) == 4 && (i + 1) < token_count
			&& get_precedence((*a)[i + 1]) < 2
			&& (aux[0] = strchr_outside_quotes((*a)[i + 1], ' ')))
		{
			aux[1] = ft_strtrim(aux[0], " \t");
			p->filename[i] = ft_substr((*a)[i + 1], 0, aux[0] - (*a)[i + 1]);
			free((*a)[i + 1]);
			(*a)[i + 1] = aux[1];
		}
	}
}

void	rearrange_tab(char ***a, char ***filename, int token_counter)
{
	char	*tmp[2];
	int		i;

	i = -1;
	while (++i < token_counter)
	{
		if ((*a)[i] == NULL)
			continue ;
		if (get_precedence((*a)[i]) == 4 && (*filename)[i])
		{
			// ft_printf_fd(1, "IN REARRANGE TAB\n");
			// ft_printf_fd(1, "a[%i]=%s\n", i, (*a)[i]);
			if ((*filename)[i] && i > 0 && get_precedence((*a)[i - 1]) < 2 && (i
					+ 1) < token_counter && get_precedence((*a)[i + 1]) < 2)
			{
				// ft_printf_fd(1, "HAI\n");
				// ft_printf_fd(1, "a[%i]=%s a[%i]=%s a[%i]=%s\n", i - 1, (*a)[i
				// - 1], i, (*a)[i], i + 1, (*a)[i + 1]);
				tmp[0] = ft_strjoin((*a)[i - 1], " ");
				tmp[1] = ft_strjoin(tmp[0], (*a)[i + 1]);
				free_all((*a)[i - 1], (*a)[i + 1], tmp[0], NULL);
				(*a)[i - 1] = tmp[1];
				(*a)[i + 1] = NULL;
				// ft_printf_fd(1, "1st if a[%i]=%s a[%i]=%s a[%i]=%s\n", i - 1,
				// (*a)[i - 1], i, (*a)[i], i + 1, (*a)[i + 1]);
			}
			else if ((i + 1) < token_counter && get_precedence((*a)[i + 1]) < 2)
			{
				tmp[0] = (*a)[i + 1];
				(*a)[i + 1] = (*a)[i];
				(*a)[i] = tmp[0];
				(*filename)[i + 1] = (*filename)[i];
				(*filename)[i] = NULL;
				// ft_printf_fd(1,
				// "2nd if a[%i]=%s a[%i]=%s filename[%i]=%s filename[%i]=%s\n",
				// i, (*a)[i], i + 1, (*a)[i + 1], i, (*filename)[i], i + 1,
				// (*filename)[i + 1]);
			}
		}
	}
}

char	**realloc_tab(char ***a, char ***filename, int token_count)
{
	char	**newtab;
	int		i;
	int		l[3];

	newtab = NULL;
	i = -1;
	l[0] = 0;
	l[1] = 0;
	while (++i < token_count)
	{
		if ((*a)[i])
		{
			l[0]++;
			if ((*filename)[i] != NULL)
				l[1]++;
		}
	}
	l[2] = l[0] + l[1];
	i = -1;
	newtab = (char **)malloc((l[2] + 1) * sizeof(char *));
	if (!newtab)
		return (NULL);
	newtab[l[2]] = NULL;
	l[0] = 0;
	while (++i < token_count)
	{
		if ((*a)[i])
		{
			newtab[l[0]] = ft_strdup((*a)[i]);
			l[0]++;
			if ((*filename)[i])
			{
				newtab[l[0]] = ft_strdup((*filename)[i]);
				l[0]++;
			}
			free_all((*a)[i], (*filename)[i], NULL, NULL);
			(*filename)[i] = NULL;
		}
	}
	return (free(*a), newtab);
}
// counts token, operators, string between non-ops as 1 token,
char	**split_by_op_ign_space_in_quote(char *input, char *ops, t_cmd_set *p)
{
	char	**a;
	int		i;

	// int		j;
	i = -1;
	a = NULL;
	p->token_count = count_tokens(input, ops);
	if (!p->token_count)
		return (NULL);
	a = (char **)malloc(sizeof(char *) * (p->token_count + 1));
	if (!a)
		return (put_err(NULL, "Malloc error", 1, p), NULL);
	fill_token_tab(input, &a, ops, p->token_count);
	// while (a[++i])
	// {
	// 	if(get_nodetype(a[i]) == N_PIPE && a[i - 1] && get_nodetype(a[i
	// 			- 1]) == N_WRITE && a[i - 2] && get_precedence(a[i - 2]) != 4)
	// 	{
	// 		j = i - 1;
	// 		free(a[j + 1]);
	// 		while (a[++j])
	// 			a[j] = a[j + 1];
	// 		free(a[j]);
	// 		p->token_count--;
	// 	}
	// }
	return (a);
}

int	check_delim_expand(char *lim)
{
	int	i;

	if (!*lim)
		return (-1);
	i = -1;
	while (lim[++i])
	{
		if (lim[i] == '\'' || lim[i] == '\"')
			return (0);
	}
	return (1);
}

int	read_heredocs(t_cmd_set *p, int *line_index, int syntax)
{
	int		i;
	char	*str[3];

	// int		expand;
	// expand = 1;
	i = -1;
	while (p->tokens[++i] && i < syntax)
	{
		// ft_printf_fd(2, "readheredocs token=%s syntax=%i\n", p->tokens[i],
		// syntax);
		if (get_nodetype(p->tokens[i]) == N_HEREDOC)
		{
			if (get_nodetype(p->tokens[i + 1]) == N_CMD)
			{
				// expand = check_delim_expand(p->tokens[i + 1]);
				str[0] = NULL;
				str[1] = NULL;
				str[2] = NULL;
				p->heredoc[i] = read_heredoc_b(str, p->tokens[i + 1], p,
						line_index);
				if (p->heredoc[i] == -1)
				{
					if (g_exit_status == 130)
						return (p->status_code = 130, 130);
					put_err("NoFile_NoDir", NULL, 1, p);
					break ;
				}
			}
			else
			{
				if (g_exit_status == 0)
					g_exit_status = p->status_code;
				return (syntax);
			}
		}
	}
	if (g_exit_status == 0)
		g_exit_status = p->status_code;
	// ft_printf_fd(2, "readheredocs end syntax=%i pstatus=%i gexit=%i\n",
	//	syntax,
	// 	p->status_code, g_exit_status);
	return (syntax);
}

int	check_syntax(char **tokens, int token_counter, t_cmd_set *p)
{
	int	i;
	int	shlvl;

	shlvl = 0;
	i = -1;
	if (token_counter > 0 && get_nodetype(tokens[0]) == N_PIPE)
		return (put_err("Unexpected_Token", tokens[0], 2, p), 0);
	while (++i < token_counter - 1)
	{
		shlvl = shlvl + (*tokens[i] == '(') - (*tokens[i] == ')');
		if (shlvl < 0)
			return (put_err("Unexpected_Token", tokens[i], 2, p), i);
		if (get_precedence(tokens[i]) == 4 && get_nodetype(tokens[i
				+ 1]) != N_CMD)
			return (put_err("Unexpected_Token", tokens[i], 2, p), (i + 1));
		else if (get_nodetype(tokens[i]) == N_CMD && *tokens[i + 1] == '(')
			return (put_err("Unexpected_Token", tokens[i], 2, p), (i + 1));
		else if (get_nodetype(tokens[i]) == N_SUBSHELL && get_nodetype(tokens[i
				+ 1]) == N_SUBSHELL && *tokens[i] != *tokens[i + 1])
			return (put_err("Unexpected_Token", tokens[i + 1], 2, p), (i + 1));
		else if (*tokens[i] == '(' && get_precedence(tokens[i + 1]) < 4
			&& get_nodetype(tokens[i + 1]) != N_CMD)
			return (put_err("Unexpected_Token", tokens[i + 1], 2, p), (i + 1));
		else if (*tokens[i] == ')' && get_nodetype(tokens[i + 1]) == N_CMD)
			return (put_err("Unexpected_Token", tokens[i + 1], 2, p), (i + 1));
		else if (get_nodetype(tokens[i]) != N_CMD
			&& get_precedence(tokens[i]) < 4 && get_nodetype(tokens[i
				+ 1]) != N_CMD && get_precedence(tokens[i + 1]) < 4)
			return (put_err("Unexpected_Token", tokens[i], 2, p), (i + 1));
		else if (get_precedence(tokens[i]) == 4 && get_precedence(tokens[i
				+ 1]) == 4)
			return (put_err("Unexpected_Token", tokens[i], 2, p), (i + 1));
	}
	shlvl = shlvl + (*tokens[i] == '(') - (*tokens[i] == ')');
	if (shlvl != 0 || (get_nodetype(tokens[i]) != N_CMD && *tokens[i] != ')'))
		return (put_err("Unexpected_Token", tokens[i], 2, p), i);
	return (999);
}

void	free_filename_tab(t_cmd_set *p)
{
	int	i;

	i = -1;
	while (++i < 1024)
		if (p->filename[i])
			free(p->filename[i]);
}
/* Processes the user input: validates quotes, tokenizes input,
parses commands, and prepares them for execution.
Also don't forget to add input to history for arry manipulation
via readline.*/
void	*process_input(char **input, t_cmd_set *p)
{
	int	line_index;
	int	len_input_arr;
	int	syntax;

	syntax = 999;
	len_input_arr = ft_arr_len(input);
	line_index = 0;
	if (!input)
		return (NULL);
	// ft_printf_fd(2, "PRE procinputwhile psttscode %i\n", p->status_code);
	while (line_index < len_input_arr)
	{
		// ft_printf_fd(2,
		// 	"processinput BEGIN leninarr=%i lineidx=%i pstatus=%i\n",
		// 	len_input_arr, line_index, p->status_code);
		syntax = 999;
		p->token_count = 0;
		p->abort = 0;
		init_heredoc_arr(p);
		if (input[line_index][0] != '\0')
			add_history(input[line_index]);
		if (check_unclosed_quotes(input[line_index]))
		{
			handle_uquote(&input[line_index], p);
			return (p);
		}
		handle_input(&input[line_index], p);
		// ft_printf_fd(2, "input=%s\n", input[line_index]);
		p->tokens = split_by_op_ign_space_in_quote(input[line_index], "<|>&()", p);
		if (!p->tokens)
			break ;
		// ft_printf_fd(1, "tokens before modifs\n");
		// print_tab(p->tokens);
		if (p->tokens && *p->tokens[0])
			syntax = check_syntax(p->tokens, p->token_count, p);
		// ft_printf_fd(1, "post SPLIT BY OP p->token_count=%i\n",
		//	p->token_count);
		// print_tab(p->tokens);
		fill_filename_tab(&p->tokens, p->token_count, p);
		// ft_printf_fd(1, "post FILL_FILENAME_TAB p->token_count=%i\n",
		// p->token_count);
		// print_tab(p->tokens);
		rearrange_tab(&p->tokens, &p->filename, p->token_count);
		// printf("after rearrange tab\n");
		// print_tab(p->tokens);
		p->tokens = realloc_tab(&p->tokens, &p->filename, p->token_count);
		// ft_printf_fd(2, "AFTER REALLOCTAB BEFORE READ HEREDOCS\n");
		// print_tab(p->tokens);
		p->token_count = ft_arr_len(p->tokens);
		// ft_printf_fd(2, "processinput syntax=%i p_statuscode=%i\n", syntax,
		// 	p->status_code);
		line_index++;
		if (read_heredocs(p, &line_index, syntax) != syntax)
		{
			// ft_printf_fd(2,
			// "postreadheredocs problem heredoc lnidx=%i len_input_arr=%i\n",
			// line_index, len_input_arr);
			//		print_tab(p->tokens);
			if (p->tokens)
				free_array(&p->tokens);
		}
		else
		{
			// ft_printf_fd(2, "postreadheredocs syntax=%i\n", syntax);
			// print_tab(p->tokens);
			if (!p->tokens)
				return (free_array(&p->tokens), p);
			//		print_tab(p->tokens);
			if (syntax != 999)
			{
				if (p->tokens)
					free_array(&p->tokens);
				// ft_printf_fd(2,
				// 	"syntax=2 p-status_code=%i gexit=%i lineidx=%i\n",
				// 	p->status_code, g_exit_status, line_index);
				continue ;
			}
			p = parse_nodes(p->tokens, p);
			if (p->tokens)
				free_array(&p->tokens);
		}
		// ft_printf_fd(2, "procinputwhile psttscode %i\n", p->status_code);
	}
	// ft_printf_fd(2, "PRE procinputwhile psttscode=%i gexit=%i\n",
	// p->status_code, g_exit_status);
	if (p->input_text)
		free_array(&p->input_text);
	if (p->filename)
		free_filename_tab(p);
	close_fds_except_std();
	return (p);
}
