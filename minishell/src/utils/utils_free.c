/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:43:42 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:37:56 by ngaurama         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->line)
		free(cmd->line);
	free_array(&(cmd->args));
	if (cmd->cmd_path)
		free(cmd->cmd_path);
	free(cmd);
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	if (node->left_node)
		free_node(node->left_node);
	if (node->right_node)
		free_node(node->right_node);
	if (node->file)
		free(node->file);
	if (node->cmd)
		free_cmd(node->cmd);
	if (node->in_fd != 0 && node->in_fd != -1)
		close(node->in_fd);
	if (node->out_fd != 1 && node->in_fd != -1)
		close(node->out_fd);
	free(node);
}

/* Frees the content of a t_cmd struct inside a linked list node.
Frees arguments and cmd_path, and closes fds if they are valid and open. */
void	free_lst(void *content)
{
	t_cmd	*node;

	node = content;
	free_array(&node->args);
	if (node->cmd_path)
		free(node->cmd_path);
	if (node->in_fd != 0 && node->in_fd != -1)
		close(node->in_fd);
	if (node->out_fd && node->out_fd != 1 && node->out_fd != -1)
		close(node->out_fd);
	if (node)
		free(node);
}

/* Frees a NULL-terminated array of strings and sets pointer to NULL.
Takes a triple pointer to free both the content and container. */
void	free_array(char ***m)
{
	int	i;

	if (!m || !*m)
		return ;
	i = 0;
	while ((*m)[i])
	{
		free((*m)[i]);
		(*m)[i] = NULL;
		i++;
	}
	free(*m);
	*m = NULL;
}

/* Frees up to four strings passed as parameters.
Each string is optional and will be freed only if not NULL. */
void	free_all(char *s1, char *s2, char *s3, char *s4)
{
	if (s1 != NULL)
		free(s1);
	if (s2 != NULL)
		free(s2);
	if (s3 != NULL)
		free(s3);
	if (s4 != NULL)
		free(s4);
}
