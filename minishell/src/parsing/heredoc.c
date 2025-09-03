/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:34:03 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 21:39:03 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*trim_all_quotes(char *str)
{
	size_t	len;
	size_t	head[2];
	char	*result;
	int		quo[3];

	head[0] = -1;
	init_quotes(quo);
	len = ft_strlen(str);
	head[0] = -1;
	result = (char *)malloc(len + 1);
	if (!result)
		return (NULL);
	head[1] = 0;
	head[0] = -1;
	init_quotes(quo);
	while (++head[0] < len)
	{
		if (!((str[head[0]] == '\'' && quo[1] == 0) || (str[head[0]] == '\"'
					&& quo[0] == 0)))
			result[head[1]++] = str[head[0]];
		upd_quo(quo, str[head[0]]);
	}
	result[head[1]] = '\0';
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

int	read_heredoc_b(char *str[3], char *lim, t_cmd_set *p, int *line_index)
{
	int	fd[2];
	int	expand;

	expand = 1;
	signal(SIGINT, signals_heredoc);
	rl_event_hook = rl_heredoc_hook;
	signal(SIGQUIT, SIG_IGN);
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
