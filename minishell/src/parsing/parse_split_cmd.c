/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_split_cmd.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:38:34 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 21:54:09 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Init one command that will be linked into a chained list.*/
t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc (sizeof * cmd);
	if (!cmd)
		return (put_err(NULL, "Malloc failed", 1, NULL));
	cmd->args = NULL;
	cmd->cmd_path = NULL;
	cmd->in_fd = 0;
	cmd->out_fd = 1;
	cmd->line = NULL;
	return (cmd);
}

/* i : current index
Normal quotes checking, if inside quotes it's only one token
note that ">>" is counted as two tokens.*/
int	size_to_malloc(char *s, char *set, int count)
{
	int	q[3];
	int	i;

	i = 0;
	init_quotes(q);
	while (s && s[i] != '\0')
	{
		count++;
		if (!ft_strchr(set, s[i]))
		{
			while ((!ft_strchr(set, s[i]) || q[2]) && s[i] != '\0')
			{
				upd_quo(q, s[i]);
				i++;
			}
			if (q[2])
				return (-1);
		}
		else if (s[i] == s[i + 1] && (s[i] == '<' || s[i] == '>' || s[i] == '&'
				|| s[i] == '|'))
			i += 2;
		else
			i++;
	}
	return (count);
}

/* ft_substr(s, i[1], i[0] - i[1]) : Allocates a new copy
starts at s + i[1] and is i[0]-i[1] characters long.
In the current loop those indices delimit the token
we have just finished scanning.
tmpstr[ i[2] ] = append to the existing array
that size been determined by size_to_malloc
i[2]++ (post-increment)	advance the write cursor.
i[0] = current scan index
i[1] = token start
i[2] = token counter*/
char	**add_to_array(char **tmpstr, char *s, char *set, int i[3])
{
	int	q[2];

	q[0] = 0;
	q[1] = 0;
	while (s && s[i[0]] != '\0')
	{
		i[1] = i[0];
		if (!ft_strchr(set, s[i[0]]))
		{
			while ((!ft_strchr(set, s[i[0]]) || q[0] || q[1]) && s[i[0]])
			{
				q[0] = (q[0] + (!q[1] && s[i[0]] == '\'')) % 2;
				q[1] = (q[1] + (!q[0] && s[i[0]] == '\"')) % 2;
				i[0]++;
			}
		}
		else
			i[0]++;
		tmpstr[i[2]++] = ft_substr(s, i[1], i[0] - i[1]);
	}
	return (tmpstr);
}

/*Initialize 3 indexs :
i[0] = current scan index
i[1] = token start
i[2] = token counter
Use size_to_malloc to check for the actual size of the array after spliting (wc)
use add_to_arry to put the actual tokens in.*/
char	**ft_split_with_pipe_or_redir_char(char const *s, char *set)
{
	char	**tmp;
	int		word_count;
	int		i[3];

	i[0] = 0;
	i[1] = 0;
	i[2] = 0;
	if (!s)
		return (NULL);
	word_count = size_to_malloc((char *)s, set, 0);
	if (word_count == -1)
		return (NULL);
	tmp = malloc((word_count + 1) * sizeof(char *));
	if (tmp == NULL)
		return (NULL);
	tmp = add_to_array(tmp, (char *)s, set, i);
	tmp[word_count] = NULL;
	return (tmp);
}

/* Main splitter for special char "<|>"
Create an array of tokens :
echo "hey" | test > ouptut.txt "ramdombulsh*t cat ls -l"
[0] echo
[1] "hey"
[2] |
[4] test
[3] >
[4] output.txt
[5] "ramdombulsh*t cat ls -l"*/
char	**split_with_special_chars(char **args)
{
	char	**subsplit;
	int		i[3];

	i[2] = -1;
	while (args && args[++i[2]])
	{
		subsplit = ft_split_with_pipe_or_redir_char(args[i[2]], "<|>&()");
		ft_array_replace(&args, subsplit, i[2]);
		i[2] += ft_arr_len(subsplit) - 1;
		free_array(&subsplit);
	}
	i[0] = -1;
	ft_printf_fd(1, "split_with_special_chars_\n");
	while (args[++i[0]])
		ft_printf_fd(1, "---args[%i]=%s\n", i[0], args[i[0]]);
	return (args);
}
