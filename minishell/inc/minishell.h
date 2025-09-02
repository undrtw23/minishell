/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alsima <alsima@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:19:14 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/02 20:30:10 by alsima           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../ft_printf_fd/ft_printf_fd.h"
# include "../get_next_line/get_next_line.h"
# include "../libft/libft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdarg.h>
# include <stdbool.h>
# include <stdio.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/wait.h>

/* Used to store the exit status of last command used
Extern so any function can use it (global variable)
Use volatile to ensure that even if the variable is modified
by signals i must not be optimized by the program
sig_atomic_t : basically modified in only ONE operation.*/
extern volatile sig_atomic_t	g_exit_status;

/* Struct which are saved in the content of a linked list for each command
args : [0] : cmd_name, [1]...[end] = NULL arguments as per execve execution.
path for exeve
in fd/out fd > redirection, piping...*/
typedef struct s_cmd
{
	char						*line;
	char						**args;
	char						*cmd_path;
	int							in_fd;
	int							out_fd;
}								t_cmd;

typedef enum s_nodetype
{
	N_INVALID,
	N_UNDEFINED,
	N_OR,        // 1
	N_AND,       // 2
	N_PIPE,      // 3
	N_READ,      // 4
	N_HEREDOC,   // 5
	N_WRITE,     // 6
	N_APPEND,    // 7
	N_READWRITE, // 8
	N_HERESTR,   // 9
	N_SUBSHELL,  // 10
	N_CMD,       // 11
}								t_nodetype;

/* node structure in order to build the AST - abstract syntax tree*/
typedef struct s_node
{
	struct s_node				*parent_node;
	t_nodetype					type;
	struct s_node				*left_node;
	struct s_node				*right_node;
	t_cmd						*cmd;
	char						*file;
	int							op_token_index;
	int							in_fd;
	int							out_fd;
	int							shlvl;
}								t_node;

/* Main structure, hold the chaine list of command
all env variables
the input text
status_code
pid of the last command.
UPDATE: holds the head of the chained list of nodes*/

typedef struct s_cmd_set
{
	char						**tokens;
	int							token_count;
	t_node						*nodes;
	char						**envp;
	char						**input_text;
	int							status_code;
	pid_t						pid_of_lst_cmd;
	int							pipe_flag;
	pid_t						pid_arr[1024];
	int							pid_index;
	char						*tmp_dir;
	int							heredoc[1024];
	char						**filename;
	int							abort;
	int							shlvl;
}								t_cmd_set;

/* nodetype enumeration, in comments arbitrary values returned in is_nodetype*/

/* Error Handling */

void							*put_err(char *err_type, char *param, int err,
									t_cmd_set *p);
void							*put_err_cd(char *err_type, char *path,
									int code, t_cmd_set *p);
void							*put_err_syntax(char *err_msg, int err_code,
									t_cmd_set *p);
void							error_token_newline(void);
void							error_unexpected_token(char *cmd);
void							error_unclosed_quotes(void);
void							error_delim_heredoc(char *lim);

/* Signal Handling */

void							signals_parent(int signal_code);
void							signals_child(int signal_code);
void							signals_heredoc(int signal_code);
int								rl_heredoc_hook(void);
void							set_signals(t_cmd_set *p);
void							disable_ctrl_z(void);

/* String and Array Manipulation */

int								ft_arr_len(char **m);
int								chrset_idx(const char *s, char *set);
int								char_index(const char *s, int c);
char							**ft_dup_array(char **m);
char							**ft_array_insert(char **in, char *newstr);
char							**ft_array_replace(char ***big, char **small,
									int n);
char							**split_and_ignore_space_if_in_quote(char *s,
									char *set);
char							*remove_quotes(char const *s1, int squote,
									int dquote);
char							*trim_all_quotes(char *str);

/* Environement manipulation */

char							*ft_getenv(char *var, char **envp);
char							**ft_setenv(char *var, char *value,
									char **envp);
int								find_env_var_index(char *var, char **envp);

/* Expand Variables*/

char							*var_expander(char *str, int *quotes,
									t_cmd_set *p);
char							*expand_variable(char *str, int i, t_cmd_set *p,
									char *s[3]);
char							*remove_dollar_quote(char *str);
char							*find_substitution(char first, char *var,
									t_cmd_set *p);
void							track_quotes(int *in_squote, int *in_dquote,
									char c);
char							*remove_char_at(char *str, size_t pos);
void							remove_curly_brackets(char **s, int i[3],
									int quotes[2], char *tmp[3]);

/* Free and Exit */

void							free_exit(t_cmd_set *p, int exit_code,
									char *msg);
void							free_lst(void *content);
void							free_all(char *s1, char *s2, char *s3,
									char *s4);
void							free_array(char ***m);
t_list							*free_tmp_lst(t_list *cmds, char **args,
									char **temp);
void							close_fds_except_std(void);

/* Initialisation */

void							init_pid_arr(t_cmd_set *p);
void							init_heredoc_arr(t_cmd_set *p);
void							init(t_cmd_set *p, char **envp, char **argv,
									int argc);

/* Command Processing and Execution */

int								check_unclosed_quotes(const char *input);
void							*process_input(char **input, t_cmd_set *p);
void							process_heredoc(char **s, int i[3],
									int quotes[2], char *tmp[3]);
void							handle_input(char **input, t_cmd_set *p);
int								update_quotes_chk_heredoc(int *quo, char ch,
									int **i, char **s);
void							find_cmd_path(t_cmd_set *p, t_node *node,
									char *path);
char							*ensure_path(char *path);
void							handle_exec_err(t_cmd *n, DIR *dir,
									t_cmd_set *p);
void							*chk_perm_call_child(t_cmd_set *p, t_list *cmd,
									int fd[2]);
void							*parse_nodes(char **args, t_cmd_set *p);
char							**split_with_special_chars(char **args);
t_cmd							*out_fd_truncate(t_cmd *node, char **args,
									int *i, t_cmd_set *p);
t_cmd							*out_fd_append(t_cmd *node, char **args, int *i,
									t_cmd_set *p);
t_cmd							*in_fd_read(t_cmd *node, char **args, int *i,
									t_cmd_set *p);
t_cmd							*in_fd_heredoc(t_cmd *node, char **args, int *i,
									t_cmd_set *p);
void							handle_env_vars(char *str, int *i, int fd[2],
									t_cmd_set *p);
void							exec_cmd_and_wait(t_cmd_set *p, int status,
									int tmp[2]);
t_cmd							*init_cmd(void);
char							*is_invalid_syntax(char **tok, int *i);
int								exec_par_builtins(t_cmd_set *p, t_list *cmd,
									int n);
void							*setup_pipe_and_fork_cmd(t_cmd_set *p,
									t_list *cmd);
void							exec_child_builtins(t_cmd_set *p, t_cmd *n,
									int l, t_list *cmd);
int								read_heredoc(char *str[3], char *lim,
									t_cmd_set *p);
int								read_heredoc_b(char *str[3], char *lim,
									t_cmd_set *p, int *line_index);
char							*get_here_str(char *str[3], char *lim,
									t_cmd_set *p, int *line_index);

/* Builtins */

int								is_builtin(t_cmd *n);
// void							builtin_exit(t_list *cmd, t_cmd_set *p);
int								builtin_exit_b(t_cmd *cmd, t_cmd_set *p);
// int								builtin_exit_child(t_list *cmd);
int								builtin_exit_child_b(t_cmd *cmd);
// int								builtin_env(char **m);
int								builtin_env_b(char **m);
// int								builtin_export(t_cmd_set *p, char **args);
int								builtin_export_b(t_cmd_set *p, char **args);
// int								builtin_unset(t_cmd_set *p, char **args);
int								builtin_unset_b(t_cmd_set *p, char **args);
// int								builtin_pwd(void);
int								builtin_pwd_b(void);
// int								builtin_echo(t_list *cmd);
int								builtin_echo_b(t_cmd *cmd);
// int								builtin_cd(t_cmd_set *p, char **cmd_args);
int								builtin_cd_b(t_cmd_set *p, char **cmd_args);

/*	Utils*/
void							upd_quo(int *quo, int c);
void							print_tab(char **tab);
int								chrset_idx(const char *s, char *set);

/*	utils_free*/

void							free_cmd(t_cmd *cmd);
void							free_node(t_node *node);
void							free_lst(void *content);
void							free_node(t_node *node);

/*	parse_nodes*/

t_nodetype						get_nodetype(char *str);
t_node							*malloc_and_init_node(t_node *node);
int								get_precedence(char *str);
int								skip_subshell(char **args, int i, int direction,
									t_cmd_set *p);
int								find_lowest_precedence_op(char **args,
									t_cmd_set *p);
void							print_tab(char **tab);
char							**trim_array(char **args, int i, int j);
char							*get_path(char **envp);
char							*get_cmd_path(char **cmd_args, char **path_tab);
t_cmd							*parse_cmd(char *str, t_cmd_set *p);
void							print_nodetype(t_node *node);
void							update_inferior_op_token_indexes(t_node *node,
									int i);
int								check_subshell(t_node *node, t_cmd_set *p,
									char **args);
t_node							*create_node(char **args, t_cmd_set *p,
									int index_base);
void							print_node(t_node *node, t_cmd_set *p);
void							*parse_nodes(char **args, t_cmd_set *p);

/*	execute_b*/

int								exec_builtins(t_node *node, t_cmd *cmd,
									t_cmd_set *p);
int								chk_inf_node_redir_perm(t_node *node,
									t_cmd_set *p);
int								check_perm_redirection(t_node *node);
void							dup2_and_close(int in_fd, int out_fd);
void							remove_token_brackets(char *file);
int								check_fd(int fd, t_node *node, t_cmd_set *p);
int								exec_node(t_node *node, t_cmd_set *p);
int								exec_cmd_node(t_node *node, t_cmd_set *p);
int								exec_node_par_builtins_b(t_node *node,
									t_cmd *cmd, t_cmd_set *p);

#endif
