/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 21:38:13 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:54:18 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	get_here_str_helper(char **str, char *lim)
{
	*str = readline("> ");
	if (g_exit_status == 130)
		return (1);
	if (!str[0])
	{
		error_delim_heredoc(lim);
		return (1);
	}
	return (0);
}

void	get_here_str_assign_str(char *str[3])
{
	if (!str[1])
		str[1] = ft_strdup("");
	if (!str[0])
		str[0] = ft_strdup("");
	str[2] = str[1];
	str[1] = ft_strjoin(str[1], str[0]);
	free_all(str[0], str[2], NULL, NULL);
}

char	*get_here_str(char *str[3], char *lim, t_cmd_set *p, int *line_index)
{
	g_exit_status = 0;
	while (g_exit_status != 130)
	{
		get_here_str_assign_str(str);
		if (p->input_text && p->input_text[*line_index])
		{
			str[0] = ft_strdup(p->input_text[*line_index]);
			(*line_index)++;
		}
		else
		{
			if (get_here_str_helper(&str[0], lim))
				break ;
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
void	process_here(char *str[1], int fd[2], t_cmd_set *p, int expand)
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

void	init_quotes(int *quo)
{
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
}
