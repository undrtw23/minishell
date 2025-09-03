/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_quote_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:22 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:18:11 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	remove_quote_and_shift(char *file, int *i, int init_len, int q[2])
{
	int	j;

	if (file[*i] == '\'')
		q[0] = !q[0];
	else
		q[1] = !q[1];
	file[*i] = 0;
	j = *i - 1;
	while (++j < init_len)
		file[j] = file[j + 1];
	(*i)--;
}

void	remove_token_brackets(char *file)
{
	int	q[2];
	int	i;
	int	init_len;

	if (!file)
		return ;
	q[0] = 0;
	q[1] = 0;
	init_len = ft_strlen(file);
	i = -1;
	while (++i < init_len)
	{
		if (should_remove_quote(file, i, q))
			remove_quote_and_shift(file, &i, init_len, q);
	}
}

int	chk_inf_node_redir_perm_helper(t_node *node, int status)
{
	if (node->type == N_READ || node->type == N_READWRITE)
		status = access(node->file, R_OK);
	if (node->type == N_WRITE || node->type == N_APPEND)
	{
		if (access(node->file, F_OK) == 0)
			status = access(node->file, W_OK);
		else
			status = 0;
	}
	return (status);
}

int	chk_inf_node_redir_perm(t_node *node, t_cmd_set *p)
{
	t_node	*left;
	int		status;

	status = 0;
	left = NULL;
	errno = 0;
	if (node->type != N_READ && node->type != N_WRITE
		&& node->type != N_READWRITE && node->type != N_APPEND)
		return (status);
	if (!(node->file))
		return (-1);
	if (node->type == N_HEREDOC)
		status = check_fd(p->heredoc[node->op_token_index], node, p);
	else
		status = chk_inf_node_redir_perm_helper(node, status);
	if (status)
		return (status);
	if (node->left_node)
		status = chk_inf_node_redir_perm(node->left_node, p);
	return (status);
}

int	handle_ambiguous_redirect(char *file_orig, char **exp, t_cmd_set *p)
{
	put_err(file_orig, "ambiguous redirect", 1, p);
	free(file_orig);
	free_array(&exp);
	return (1);
}
