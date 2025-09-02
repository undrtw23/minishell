/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_nodes.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:37:49 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/02 22:15:29 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Processes one token at index i, checking for redirections or arguments.
Handles append/truncate/redirect/pipe tokens and errors.
Updates the command list or prints an error if invalid.*/
// static t_cmd	*check_redir_pipe(t_cmd *t, char **a[2], int *i, t_cmd_set *p)
// {
// 	char	*err;

// 	err = is_invalid_syntax(a[0], i);
// 	if (err && ft_strncmp(err, "eof", 3))
// 		return (*i = -1, put_err("Unexpected_Token", err, 2, p), t);
// 	else if (err)
// 		return (*i = -1, put_err_syntax(err, 2, p), t);
// 	if (a[0][*i][0] == '>' && a[0][*i + 1] && a[0][*i + 1][0] == '>'
// 		&& g_exit_status != 130)
// 		t = out_fd_append(t, a[1], i, p);
// 	else if (a[0][*i][0] == '>' && g_exit_status != 130)
// 		t = out_fd_truncate(t, a[1], i, p);
// 	else if (a[0][*i][0] == '<' && a[0][*i + 1] && a[0][*i + 1][0] == '<')
// 		t = in_fd_heredoc(t, a[1], i, p);
// 	else if (a[0][*i][0] == '<')
// 		t = in_fd_read(t, a[1], i, p);
// 	else if (a[0][*i][0] != '|')
// 		t->args = ft_array_insert(t->args, a[1][*i]);
// 	return (t);
// }

/* Create the array with argument without quotes for concrete building.	*/
// static char	**args_after_quotes_removed(char **args)
// {
// 	char	**temp;
// 	char	*tmpstr;
// 	int		j;

// 	j = -1;
// 	temp = ft_dup_array(args);
// 	while (temp && temp[++j])
// 	{
// 		tmpstr = remove_quotes(temp[j], 0, 0);
// 		free(temp[j]);
// 		temp[j] = tmpstr;
// 	}
// 	return (temp);
// }

/* Actual list builders that will give a chaine list of command to t_cmd_set.
temp[0] : actual array with quotes
temp[1] : without quotes (useed for storing)
cmds[0] head of list (that will be returned)
cmds[1] : actual command being processed
if we encounter a pipe, and a command is after, create a new cmd
cmds[1] = ft_lstlast(cmds[0]); append it at the end of list*/
// static t_list	*parse_cmds(char **args, int i, t_cmd_set *p)
// {
// 	t_list	*cmds[2];
// 	char	**temp[2];

// 	cmds[0] = NULL;
// 	if (args && args[0] && args[0][0] == '|')
// 		return (put_err("Unexpected_Token", "|", 2, p), free_tmp_lst(NULL, args,
// 				NULL));
// 	temp[1] = args_after_quotes_removed(args);
// 	while (args[++i])
// 	{
// 		if (i == 0 || (args[i][0] == '|' && args[i + 1] && args[i + 1][0]))
// 		{
// 			i += args[i][0] == '|';
// 			ft_lstadd_back(&cmds[0], ft_lstnew(init_cmd()));
// 		}
// 		cmds[1] = ft_lstlast(cmds[0]);
// 		temp[0] = args;
// 		cmds[1]->content = check_redir_pipe(cmds[1]->content, temp, &i, p);
// 		if (i < 0)
// 			return (free_tmp_lst(cmds[0], args, temp[1]));
// 	}
// 	return (free_array(&temp[1]), free_array(&args), cmds[0]);
// }
// N_OR // 1
// N_AND // 2
// N_PIPE // 3
// N_READ // 4
// N_HEREDOC // 5
// N_WRITE // 6
// N_APPEND // 7
// N_GROUP // 8
// N_CMD // 10

t_nodetype	get_nodetype(char *str)
{
	int	l;

	l = ft_strlen(str);
	if (!str)
		return (N_INVALID);
	if (!ft_strncmp("||", str, 2) && l == 2)
		return (N_OR);
	if (!ft_strncmp("&&", str, 2) && l == 2)
		return (N_AND);
	if (!ft_strncmp("|", str, 1) && l == 1)
		return (N_PIPE);
	if (!ft_strncmp("<<", str, 2) && l == 2)
		return (N_HEREDOC);
	if (!ft_strncmp("<", str, 1) && l == 1)
		return (N_READ);
	if (!ft_strncmp(">>", str, 2) && l == 2)
		return (N_APPEND);
	if (!ft_strncmp(">", str, 1) && l == 1)
		return (N_WRITE);
	if (!ft_strncmp("<>", str, 2) && l == 2)
		return (N_READWRITE);
	if (!ft_strncmp("<<<", str, 3) && l == 3)
		return (N_HERESTR);
	if ((!ft_strncmp("(", str, 1) || !ft_strncmp(")", str, 1)) && l == 1)
		return (N_SUBSHELL);
	else
		return (N_CMD);
}

t_node	*malloc_and_init_node(t_node *node)
{
	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->cmd = NULL;
	node->type = N_UNDEFINED;
	node->parent_node = NULL;
	node->left_node = NULL;
	node->right_node = NULL;
	node->file = NULL;
	node->in_fd = 0;
	node->out_fd = 1;
	node->op_token_index = 0;
	node->shlvl = 0;
	return (node);
}
//	precedence in nodetypes is used to build AST abstract tree syntax:
//	cmd<and=or<pipe<redirect<subshell
int	get_precedence(char *str)
{
	t_nodetype	type;

	type = get_nodetype(str);
	if (type == N_SUBSHELL)
		return (5);
	if (type == N_WRITE || type == N_APPEND || type == N_READ
		|| type == N_HEREDOC || type == N_READWRITE || type == N_HERESTR)
		return (4);
	if (type == N_PIPE)
		return (3);
	if (type == N_AND || type == N_OR)
		return (2);
	else
		return (0);
}
//	finds the opposing parenthesis in the args list. if direction is -1,
//	it goes from a closing ) to its opening (,
//	ie towards the start of the args array.
int	skip_subshell(char **args, int i, int direction, t_cmd_set *p)
{
	int	k;
	int	l;
	int	old_i;

	old_i = i;
	l = ft_strlen(args[i]);
	k = 0;
	if (direction == -1)
	{
		if (!ft_strncmp(args[i], "(", 1))
		{
			return (put_err("Unexpected_Token", "\\n", 2, p), -1);
		}
		if (args[i - 1] && get_precedence(args[i - 1]) >= 2 && *args[i
			- 1] != ')')
			return (put_err("Unexpected_Token", args[old_i], 2, p), -1);
		while (i >= 0 && args[i])
		{
			if (!ft_strncmp(args[i], ")", 1))
				k--;
			else if (!ft_strncmp(args[i], "(", 1))
				k++;
			if (k == 0)
			{
				if (args[i + 1] && get_precedence(args[i + 1]) >= 2 && *args[i
					+ 1] != '(')
					return (put_err("Unexpected_Token", args[old_i], 2, p), -1);
				return (i);
			}
			i--;
		}
	}
	else
	{
		while (i < l && args[i])
		{
			if (!ft_strncmp(args[i], ")", 1))
				k--;
			else if (ft_strncmp(args[i], "(", 1))
				k++;
			if (k == 0)
				return (i);
			i++;
		}
	}
	return (put_err("Unexpected_Token", args[old_i], 2, p), -1);
	// ft_printf_fd(1, "\t\tIN skipSubshell k=%d i=%d \n", k, i);
	return (-1);
}

int	find_lowest_precedence_op(char **args, t_cmd_set *p)
{
	int	l;
	int	i;
	int	x;
	int	low;

	x = -1;
	low = 6; // SUBSHELL has the highest precedence operator which is 5
	// ft_printf_fd(1, "\tIN find lowest precedence op\n");
	// print_tab(args);
	l = ft_arr_len(args);
	// ft_printf_fd(1, "\tft_arr_len args = %d\n", l);
	i = l;
	while (--i >= 0)
	{
		// ft_printf_fd(1,
		// "in FindLowestPrecedence args[%d]=%s getprecedence=%d\n", i,
		// args[i], low);
		if (get_nodetype(args[i]) != N_CMD && get_precedence(args[i]) < low)
		{
			low = get_precedence(args[i]);
			x = i;
			// ft_printf_fd(1,
			//	"in FindLowestPrecedence args[%d]=%s newlow=%d\n",
			// i, args[i], low);
		}
		if (get_nodetype(args[i]) == N_SUBSHELL)
		{
			// ft_printf_fd(1, "\tfound subshell i=%d\n", i);
			if (skip_subshell(args, i, -1, p) == -1)
				return (-1);
			i = skip_subshell(args, i, -1, p);
			// ft_printf_fd(1, "\ti becomes %d\n", i);
		}
	}
	if (low == 6)
		return (0); // means there are no ops, only cmd
	else
		return (x);
	// returns the index where the op with the least precedence is
}
void	print_tab(char **tab)
{
	int	i;

	ft_printf_fd(1, "\tPRINT_TAB\n");
	i = 0;
	if (!tab)
		ft_printf_fd(1, "\ttab is NULL\n");
	else
	{
		while (tab[i])
		{
			ft_printf_fd(1, "\ttab[%i]=%s\n", i, tab[i]);
			i++;
		}
	}
}
// trim array args from i to j including i and j
char	**trim_array(char **args, int i, int j)
{
	char	**newtab;
	int		k[2];

	k[0] = 0;
	k[1] = ft_arr_len(args);
	if (!(i >= 0 && i <= j && j <= k[1]))
	{
		//  ft_printf_fd(1, "trim_array WTF i=%d, j=%d arr_len=%d\n", i, j,
		//  k[1]);
		return (NULL);
	}
	k[1] = j - i + 1;
	//  ft_printf_fd(1, "TrimArray i=%d, j=%d arr_len=%d\n", i, j, k[1]);
	newtab = (char **)malloc((k[1] + 1) * sizeof(char *));
	while (k[0] < k[1])
	{
		newtab[k[0]] = ft_strdup(args[i + k[0]]);
		//  ft_printf_fd(1, "args[%d]=%s and newtab[%d]=%s\n", i + k[0], args[i
		//  + k[0]], k[0], newtab[k[0]]);
		k[0]++;
	}
	newtab[k[0]] = NULL;
	// print_tab(newtab);
	return (newtab);
}

char	*get_path(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp("PATH", envp[i], 4) == 0)
			return (envp[i] + 5);
	}
	return (NULL);
}

char	*get_cmd_path(char **cmd_args, char **path_tab)
{
	char	*str;
	char	*aux;
	int		i;

	if (!cmd_args || !cmd_args[0])
		return (NULL);
	if (access(cmd_args[0], F_OK) == 0)
		return (ft_strdup(cmd_args[0]));
	i = -1;
	aux = ft_strjoin("/", cmd_args[0]);
	while (path_tab[++i])
	{
		str = ft_strjoin(path_tab[i], aux);
		if (access(str, F_OK) == 0)
		{
			free(aux);
			return (str);
		}
		free(str);
	}
	free(aux);
	return (NULL);
}
t_cmd	*parse_cmd(char *str, t_cmd_set *p)
{
	t_cmd	*cmd;

	//	int		i;
	//	char	**path_tab;
	//	char	*path;
	(void)p;
	// remove_token_brackets(str);
	cmd = init_cmd();
	cmd->line = ft_strdup(str);
	// ft_printf_fd(2, "parsecmd cmd->line=%s\n", cmd->line);
	//	cmd->args = split_and_ignore_space_if_in_quote(str, " \t");
	//	print_tab(cmd->args);
	//	i = -1;
	//	while (cmd->args[++i])
	//		remove_token_brackets(cmd->args[i]);
	// print_tab(cmd->args);
	//	path = ft_getenv("PATH", p->envp);
	// ft_printf_fd(1, "path=%s\n", path);
	//	path_tab = ft_split(path, ':');
	// print_tab(path_tab);
	//	cmd->cmd_path = get_cmd_path(cmd->args, path_tab);
	//	free_array(&path_tab);
	//	free(path);
	//	cmd->cmd_path = ensure_path(cmd->cmd_path);
	// ft_printf_fd(1, "parse_cmd4 returning cmd\n");
	return (cmd);
}
void	print_nodetype(t_node *node)
{
	if (node->type == N_SUBSHELL)
		ft_printf_fd(1, "SUBSHELL\n");
	else if (node->type == N_APPEND)
		ft_printf_fd(1, "N_APPEND\n");
	else if (node->type == N_WRITE)
		ft_printf_fd(1, "N_WRITE\n");
	else if (node->type == N_READ)
		ft_printf_fd(1, "N_READ\n");
	else if (node->type == N_HEREDOC)
		ft_printf_fd(1, "N_HEREDOC\n");
	else if (node->type == N_HERESTR)
		ft_printf_fd(1, "N_HERESTR\n");
	else if (node->type == N_PIPE)
		ft_printf_fd(1, "N_PIPE\n");
	else if (node->type == N_AND)
		ft_printf_fd(1, "N_AND\n");
	else if (node->type == N_OR)
		ft_printf_fd(1, "N_OR\n");
	else if (node->type == N_CMD)
		ft_printf_fd(1, "N_CMD\n");
}

void	update_inferior_op_token_indexes(t_node *node, int i)
{
	if (node->left_node)
		update_inferior_op_token_indexes(node->left_node, i);
	if (node->right_node)
		update_inferior_op_token_indexes(node->right_node, i);
	node->op_token_index += i;
}
int	check_subshell(t_node *node, t_cmd_set *p, char **args)
{
	int	len;
	int	i;
	int	sh_head[2];

	// TODO rezolva sa fie op inainte si dupa subshell, altfel put_err!
	// cat < x ( grep b)
	(void)node;
	i = -1;
	len = ft_arr_len(args);
	while (++i < len && ft_strncmp(args[i], "(", 1))
		;
	sh_head[0] = i;
	i = len;
	while (--i >= 0 && ft_strncmp(args[i], ")", 1))
		;
	sh_head[1] = i;
	if (sh_head[0] > 0 && get_precedence(args[sh_head[0] - 1]) < 2)
		return (put_err("Unexpected_Token1", args[sh_head[0] + 1], 2, p), 2);
	else if (sh_head[1] - sh_head[0] == 1)
		return (put_err("Unexpected_Token2", args[sh_head[1]], 2, p), 2);
	else if (sh_head[0] == 0 && (sh_head[1] + 1 < len)
		&& get_precedence(args[sh_head[1] + 1]) < 2)
		return (put_err("Unexpected_Token3", args[sh_head[1] + 1], 2, p), 2);
	else
		return (0);
}

int	check_redirect_node(char **args, int i, t_node *nodehead, t_cmd_set *p)
{
	(void)p;
	if (args[i + 1] && (get_nodetype(args[i + 1]) == N_CMD))
	{
		nodehead->file = ft_strdup(args[i + 1]);
		// remove_token_brackets(nodehead->file);
		// ft_printf_fd(1, "node->file=~%s~\n", nodehead->file);
	}
	else
	{
		// ft_printf_fd(1, "p->tokens[nodehead->op_token_index=%s\n",
		// p->tokens[nodehead->op_token_index]);
		put_err("Unexpected_Token", p->tokens[nodehead->op_token_index + 1], 2,
			p);
		return (1);
	}
	if (p->tokens[nodehead->op_token_index + 2]
		&& !ft_strncmp(p->tokens[nodehead->op_token_index + 2], "(", 1))
	{
		// ft_printf_fd(1, "in check redirectnode???\n");
		put_err("Unexpected_Token", p->tokens[nodehead->op_token_index + 2], 2,
			p);
		return (1);
	}
	else
		return (0);
}

t_node	*create_node(char **args, t_cmd_set *p, int index_base)
{
	int		i;
	t_node	*nodehead;

	nodehead = NULL;
	if (!args || !*args || !*args[0])
		return (NULL);
	nodehead = malloc_and_init_node(nodehead);
	if (nodehead == NULL)
		return (NULL);
	// ft_printf_fd(1, "\n------ CREATE_NODE ------\n");
	i = find_lowest_precedence_op(args, p);
	if (i == -1)
		return (free_array(&args), free_node(nodehead), NULL);
	// ft_printf_fd(1,"IN createNode after findlowestprec\nlowest:args[%d]=%s\n",
	// 	i, args[i]);
	nodehead->type = get_nodetype(args[i]);
	nodehead->op_token_index = i + index_base;
	// ft_printf_fd(1, "op_token_index = %i\n", nodehead->op_token_index);
	// ft_printf_fd(1, "nodetype = ");
	// print_nodetype(nodehead);
	if (nodehead->type == N_CMD)
	{
		nodehead->cmd = parse_cmd(args[i], p);
		return (free_array(&args), nodehead);
	}
	else if (get_precedence(args[i]) == 4)
	{
		if (check_redirect_node(args, i, nodehead, p))
			return (free_node(nodehead), free_array(&args), NULL);
		// ft_printf_fd(1, "PREC4 args[%i+1]=%s, getprec(i+1)=%i\n", i, args[i
		// 	+ 1], get_precedence(args[i + 1]));
		// heredoc delim stored as node->file
		if (i != 0)
		{
			nodehead->left_node = create_node(trim_array(args, 0, i - 1), p,
					index_base);
			// if (nodehead->left_node == NULL && nodehead->type != N_READWRITE)
			// 	return (free_node(nodehead), free_array(&args), NULL);
		}
		else
		{
			nodehead->left_node = create_node(trim_array(args, i + 2,
						ft_arr_len(args) - 1), p, index_base);
			// if (nodehead->left_node == NULL && nodehead->type != N_READWRITE)
			// 	return (free_node(nodehead), free_array(&args), NULL);
			//	if (nodehead->right_node == NULL)
			// ft_printf_fd(1, "nodeheadnull\n");
			// if (nodehead->right_node == NULL)
			// 	return (free_node(nodehead), free_array(&args), NULL);
		}
		if (nodehead->left_node)
			nodehead->left_node->parent_node = nodehead;
		free_array(&args);
		return (nodehead);
	}
	else if (nodehead->type == N_SUBSHELL)
	{
		// ft_printf_fd(1,
		// 	"IN CREATENODE ELSEIF TYPE=SUBSHELL p->tokens[node->optokenidx]=%s\n",
		// 	p->tokens[nodehead->op_token_index]);
		if (check_subshell(nodehead, p, args))
			return (free_node(nodehead), free_array(&args), NULL);
		nodehead->left_node = create_node(trim_array(args, 1, i - 1), p, 1
				+ index_base);
		if (nodehead->left_node == NULL)
			return (free_node(nodehead), free_array(&args), NULL);
		nodehead->left_node->parent_node = nodehead;
		return (free_array(&args), nodehead);
	}
	else
		nodehead->left_node = create_node(trim_array(args, 0, i - 1), p,
				index_base);
	if (nodehead->left_node == NULL)
		return (free_node(nodehead), free_array(&args), NULL);
	nodehead->left_node->parent_node = nodehead;
	// ft_printf_fd(1, "------going to rightNode------\n");
	///	print_tab(args);
	//	ft_printf_fd(1, "i+1=%d arr_len=%d\n", i + 1, ft_arr_len(args));
	nodehead->right_node = create_node(trim_array(args, i + 1, ft_arr_len(args)
				- 1), p, i + 1 + index_base);
	if (nodehead->right_node == NULL)
		return (free_node(nodehead), free_array(&args), NULL);
	nodehead->right_node->parent_node = nodehead;
	//	update_inferior_op_token_indexes(nodehead->right_node, i + 1);
	//	free_array(&args);
	return (free_array(&args), nodehead);
}

void	print_node(t_node *node, t_cmd_set *p)
{
	ft_printf_fd(1, "BEGIN node\n");
	ft_printf_fd(1, "p[%i]=%s\n", node->op_token_index,
		p->tokens[node->op_token_index]);
	ft_printf_fd(1, "nodetype: ");
	if (node->type == N_SUBSHELL)
		ft_printf_fd(1, "SUBSHELL\n");
	else if (node->type == N_APPEND)
		ft_printf_fd(1, "N_APPEND\n");
	else if (node->type == N_WRITE)
		ft_printf_fd(1, "N_WRITE\n");
	else if (node->type == N_READ)
		ft_printf_fd(1, "N_READ\n");
	else if (node->type == N_HEREDOC)
		ft_printf_fd(1, "N_HEREDOC\n");
	else if (node->type == N_READWRITE)
		ft_printf_fd(1, "N_READWRITE\n");
	else if (node->type == N_HERESTR)
		ft_printf_fd(1, "N_HERESTR\n");
	else if (node->type == N_PIPE)
		ft_printf_fd(1, "N_PIPE\n");
	else if (node->type == N_AND)
		ft_printf_fd(1, "N_AND\n");
	else if (node->type == N_OR)
		ft_printf_fd(1, "N_OR\n");
	else if (node->type == N_CMD)
	{
		ft_printf_fd(1, "N_CMD\n");
		ft_printf_fd(1, "cmd->line=%s\n", node->cmd->line);
		ft_printf_fd(1, "END node ");
		print_nodetype(node);
		return ;
	}
	if (node->parent_node)
	{
		ft_printf_fd(1, "parent_node_type: ");
		if (node->parent_node->type == N_SUBSHELL)
			ft_printf_fd(1, "SUBSHELL\n");
		else if (node->parent_node->type == N_APPEND)
			ft_printf_fd(1, "N_APPEND\n");
		else if (node->parent_node->type == N_WRITE)
			ft_printf_fd(1, "N_WRITE\n");
		else if (node->parent_node->type == N_READ)
			ft_printf_fd(1, "N_READ\n");
		else if (node->parent_node->type == N_HEREDOC)
			ft_printf_fd(1, "N_HEREDOC\n");
		else if (node->parent_node->type == N_READWRITE)
			ft_printf_fd(1, "N_READWRITE\n");
		else if (node->parent_node->type == N_HERESTR)
			ft_printf_fd(1, "N_HERESTR\n");
		else if (node->parent_node->type == N_PIPE)
			ft_printf_fd(1, "N_PIPE\n");
		else if (node->parent_node->type == N_AND)
			ft_printf_fd(1, "N_AND\n");
		else if (node->parent_node->type == N_OR)
			ft_printf_fd(1, "N_OR\n");
		else if (node->parent_node->type == N_CMD)
			ft_printf_fd(1, "N_CMD\n");
	}
	if (node->file)
		ft_printf_fd(1, "filename: %s\n", node->file);
	if (node->left_node)
	{
		ft_printf_fd(1, "left_node\n");
		print_node(node->left_node, p);
	}
	else
		ft_printf_fd(1, "left_node NULL\n");
	if (node->right_node)
	{
		ft_printf_fd(1, "right_node\n");
		print_node(node->right_node, p);
	}
	else
		ft_printf_fd(1, "right_node NULL\n");
	ft_printf_fd(1, "END node ");
	print_nodetype(node);
}

// Parses and executes the given arguments. Uses parse_cmds to build the command list.Sets the $_ variable and calls exec_cmd_and_wait.Returns the original pointer after execution.*/

void	*parse_nodes(char **args, t_cmd_set *p)
{
	int		status;
	int		i;
	int		j;
	char	**dup_args;

	dup_args = ft_dup_array(args);
	status = 0;
	i = 0;
	j = -1;
	p->nodes = create_node(dup_args, p, 0);
	if (p->nodes == NULL)
		return (p);
	// ft_printf_fd(1, "++++++++++++ PRINT_NODES ++++++++++++\n");
	// print_node(p->nodes, p);
	// ft_printf_fd(1, "************ EXEC_NODES ************\n");
	// ft_printf_fd(2, "before exec p->statuscode=%i\n", p->status_code);
	status = exec_node(p->nodes, p);
	// ft_printf_fd(2, "post exec p->statuscode=%i while status=%i\n",
	// 	p->status_code, status);
	// if (status)
	// 	return (free_node(p->nodes), p->nodes = NULL, p);
	free_node(p->nodes);
	p->nodes = NULL;
	g_exit_status = p->status_code;
	return (p);
}
