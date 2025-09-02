/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:34:03 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/02 20:58:10 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Function ta accumulate the raw heredoc text (reading).
Run until Ctrl+C (SIGINT) is threw.
str[0] = current input line
str[1] = accumulated buffer
str[2] = scratch pointer
Ensure that pointer are not NULL
str[0] = readline("> "); prompt the user to continue writing.
if a line is matching the delimiter, break.
otherwise, append newline and continue.
Return the accumulated text.*/
char	*get_here_str(char *str[3], char *lim, t_cmd_set *p, int *line_index)
{
	g_exit_status = 0;
	while (g_exit_status != 130)
	{
		if (!str[1])
			str[1] = ft_strdup("");
		if (!str[0])
			str[0] = ft_strdup("");
		str[2] = str[1];
		str[1] = ft_strjoin(str[1], str[0]);
		free_all(str[0], str[2], NULL, NULL);
		if (p->input_text && p->input_text[*line_index])
		{
			str[0] = ft_strdup(p->input_text[*line_index]);
			(*line_index)++;
		}
		else
		{
			str[0] = readline("> ");
			if (g_exit_status == 130)
				break ;
			if (!str[0])
			{
				error_delim_heredoc(lim);
				break ;
			}
		}
		str[2] = str[0];
		if (str[2] && !ft_strncmp(str[2], lim, ft_strlen(str[2]))
			&& ft_strlen(str[2]) == ft_strlen(lim))
			break ;
		str[0] = ft_strjoin(str[0], "\n");
		free(str[2]);
	}
	return (free_all(str[0], NULL, NULL, NULL), str[1]);
}

/* Writing tool for when the heredoc finished.
str[1] is the final heredoc buffer from get_here_str
fd[1] is the write end of the heredoc pipe
expand = 0 -> do not expand variables.*/
static void	process_here(char *str[1], int fd[2], t_cmd_set *p, int expand)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str && str[1] && str[1][i])
	{
		j = i + 1;
		if (str[1][i] && str[1][i] == '$' && str[1][j] && ft_isalnum(str[1][j])
			&& expand)
			handle_env_vars(str[1], &i, fd, p);
		else if (str[1][i])
			write(fd[1], &str[1][i], 1);
		i++;
	}
}

/* Remove quotes from the delimiter if they exist.*/
char	*trim_all_quotes(char *str)
{
	size_t	len;
	size_t	result_len;
	size_t	i;
	char	*result;
	size_t	j;
	int		quo[3];

	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	len = ft_strlen(str);
	result_len = 0;
	i = -1;
	while (++i < len)
	{
		if (!((str[i] == '\'' && quo[1] == 0) || (str[i] == '\"'
					&& quo[0] == 0)))
			result_len++;
		upd_quo(quo, str[i]);
	}
	result = (char *)malloc(result_len + 1);
	if (!result)
	{
		return (NULL);
	}
	j = 0;
	i = -1;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	while (++i < len)
	{
		if (!((str[i] == '\'' && quo[1] == 0) || (str[i] == '\"'
					&& quo[0] == 0)))
			result[j++] = str[i];
		upd_quo(quo, str[i]);
	}
	result[j] = '\0';
	return (result);
}

/* Main function :
Signal handling for SIGINT, ignore ctr + \ in heredoc.
if delimiter is quoted, expand = 1, 0 else.
When writing done, close fd
If aborted by ctrl + c -> close rear-end.*/
int	read_heredoc(char *str[3], char *lim, t_cmd_set *p)
{
	int	fd[2];
	int	expand;

	expand = 1;
	signal(SIGINT, signals_heredoc);
	signal(SIGQUIT, SIG_IGN);
	if (pipe(fd) == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p), -1);
	if (chrset_idx(lim, "\"'") != -1)
		expand = 0;
	lim = trim_all_quotes(lim);
	str[1] = get_here_str(str, lim, p, NULL);
	process_here(str, fd, p, expand);
	free_all(str[1], lim, NULL, NULL);
	if (fd[1] != -1)
		close(fd[1]);
	if (p->status_code == 130)
		return (close(fd[0]), -1);
	return (fd[0]);
}

void	trim_ext_squotes(char *lim)
{
	int	len;
	int	i;

	len = ft_strlen(lim);
	if (!ft_strncmp(lim, "''", 2) && len == 2)
	{
		lim[0] = '\0';
		lim[1] = '\0';
		return ;
	}
	i = -1;
	while (++i < len - 2)
	{
		lim[i] = lim[i + 1];
	}
	lim[i++] = '\0';
	lim[i] = '\0';
}
/* new read_heredoc */
int	read_heredoc_b(char *str[3], char *lim, t_cmd_set *p, int *line_index)
{
	int	fd[2];
	int	expand;
	

	// ft_printf_fd(2, "readheredoc lim=%s\n", lim);
	expand = 1;
	signal(SIGINT, signals_heredoc);
	rl_event_hook = rl_heredoc_hook;
	signal(SIGQUIT, SIG_IGN);
	// signal(SIGINT, SIG_DFL);
	if (pipe(fd) == -1)
		return (put_err("DupForkPipe_Failed", NULL, 1, p), -1);
	trim_ext_squotes(lim);
	if (chrset_idx(lim, "\"\'") != -1)
		expand = 0;
	lim = trim_all_quotes(lim);
	str[1] = get_here_str(str, lim, p, line_index);
	process_here(str, fd, p, expand);
	free_all(str[1], lim, NULL, NULL);
	if (fd[1] != -1)
		close(fd[1]);
	if (g_exit_status == 130)
		return (close(fd[0]), -1);
	return (fd[0]);
}

/*Parses a here-document redirection (‘<< LIMIT’) at index *i in args,
collects the heredoc input via	read_heredoc(void), and stores the resulting
read-end file descriptor in node->in_fd.
If no limiter or an error, aborts parsing by setting *i = -1
and, on syntax error, prints an unexpected newline token message.
Returns the modified node for chaining in the parser.*/
t_cmd	*in_fd_heredoc(t_cmd *node, char **args, int *i, t_cmd_set *p)
{
	char	*str[3];
	char	*lim;

	str[0] = NULL;
	str[1] = NULL;
	str[2] = NULL;
	(*i)++;
	if (args[++(*i)])
	{
		lim = args[*i];
		node->in_fd = read_heredoc(str, lim, p);
	}
	if (!args[*i] || node->in_fd == -1)
	{
		*i = -1;
		if (node->in_fd != -1)
		{
			ft_putstr_fd("mini :", 2);
			ft_putendl_fd("syntax error near unexpected token `newline'", 2);
			p->status_code = 2;
		}
	}
	return (node);
}
