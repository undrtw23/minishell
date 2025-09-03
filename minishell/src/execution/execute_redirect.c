/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redirect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 20:42:36 by ngaurama          #+#    #+#             */
/*   Updated: 2025/09/03 21:18:31 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_redirect_type(t_node *node, t_cmd_set *p)
{
	if (node->type == N_READ)
		return (exec_read_node(node, p));
	if (node->type == N_HEREDOC)
		return (exec_heredoc_node(node, p));
	if (node->type == N_WRITE || node->type == N_APPEND)
		return (exec_write_or_append_node(node, p));
	if (node->type == N_READWRITE)
		return (exec_readwrite_node(node, p));
	if (node->type == N_HERESTR)
		return (exec_herestr_node(node, p));
	return (-1);
}

int	exec_redirect_node(t_node *node, t_cmd_set *p)
{
	char	**exp;
	char	*file_orig;
	int		quo[3];
	int		i;

	exp = NULL;
	quo[0] = 0;
	quo[1] = 0;
	quo[2] = 0;
	file_orig = ft_strdup(node->file);
	node->file = remove_dollar_quote(node->file);
	node->file = var_expander(node->file, quo, p);
	exp = split_and_ignore_space_if_in_quote(node->file, " \t");
	i = -1;
	while (exp[++i])
		exp = glob_expander(&exp, &i, quo, p);
	if (ft_arr_len(exp) > 1 || !*node->file)
		return (handle_ambiguous_redirect(file_orig, exp, p));
	free(node->file);
	node->file = ft_strdup(exp[0]);
	free_array(&exp);
	free(file_orig);
	return (handle_redirect_type(node, p));
}

void	init_subshell(t_cmd_set *p, t_node *node)
{
	int	i;

	(void)node;
	p->abort = 0;
	p->pipe_flag = 0;
	i = -1;
	while (++i < 4096)
		p->pid_arr[i] = 0;
	p->pid_index = 0;
	p->shlvl++;
}

void	check_for_heredoc_close_fd(t_node *node, t_cmd_set *p)
{
	if (node->type == N_HEREDOC)
		close(p->heredoc[node->op_token_index]);
	if (node->left_node)
		check_for_heredoc_close_fd(node->left_node, p);
	if (node->right_node)
		check_for_heredoc_close_fd(node->right_node, p);
}

void	handle_parent_process(pid_t pid, t_node *node, t_cmd_set *p)
{
	p->pid_arr[p->pid_index] = pid;
	p->pid_index++;
	check_for_heredoc_close_fd(node, p);
	p->pid_of_lst_cmd = pid;
}
