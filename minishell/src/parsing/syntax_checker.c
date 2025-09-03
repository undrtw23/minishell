/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 13:16:17 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:48:06 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Checks if the character c is a redirection operator ('<' or '>').
Returns 1 if it is, 0 otherwise. */
int	is_redir(char c)
{
	char	c2;

	c2 = c;
	if (c2 == '<' || c2 == '>')
		return (1);
	return (0);
}

/* Checks for syntax errors related to the pipe ('|') token at index idx.
Returns "|" for a leading pipe or two consecutive pipes, 
"eof" for a pipe at the end,
or NULL if there is no pipe-related syntax error. */
char	*pipe_error(char **tok, int idx)
{
	char	*next;

	next = tok[idx + 1];
	if (tok[idx][0] == '|' && idx == 0)
		return ("|");
	if (tok[idx][0] == '|' && (!next || next[0] == '\0'))
		return ("eof");
	if (tok[idx][0] == '|' && next && next[0] == '|')
		return ("|");
	if (is_redir(tok[idx][0]) && next && next[0] == '|')
		return ("|");
	return (NULL);
}

/* Checks for syntax errors related to redirection tokens
 ('<' or '>') at index idx.
Returns error codes for too many consecutive redirection tokens,
 missing arguments,
invalid combinations like '<>', or redirection followed by a pipe.
Returns NULL if there is no redirection-related syntax error. */
char	*redir_error(char **tok, int idx)
{
	char	c;
	int		n;
	char	*next;

	c = tok[idx][0];
	n = 0;
	next = tok[idx + 1];
	while (tok[idx + n] && tok[idx + n][0] == c)
		n++;
	if (c == '<' && n == 3)
		return ("newline");
	if (n > 2 && c == '>')
		return (">");
	if (n > 2)
		return ("<");
	if (n == 2 && (!next || next[0] == '\0' || next[0] == '|'))
		return ("newline");
	if (n == 1 && (!next || is_redir(next[0]) || next[0] == '|'))
		return ("newline");
	if (c == '>' && next && next[0] == '<')
		return ("<");
	if (c == '<' && next && next[0] == '>')
		return ("newline");
	return (NULL);
}

/* Checks for invalid shell syntax at the current token index.
Calls pipe_error and redir_error as needed.
Returns a string indicating the error type if found,
 or NULL if syntax is valid. */
char	*is_invalid_syntax(char **tok, int *i)
{
	char	*err;
	char	c;

	if (!tok[*i])
		return ("newline");
	c = tok[*i][0];
	err = pipe_error(tok, *i);
	if (err != NULL)
		return (err);
	if (is_redir(c))
	{
		err = redir_error(tok, *i);
		if (err != NULL)
			return (err);
	}
	return (NULL);
}
