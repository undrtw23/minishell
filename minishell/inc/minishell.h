/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 18:19:14 by gkorzecz          #+#    #+#             */
/*   Updated: 2025/09/03 22:57:20 by ngaurama         ###   ########.fr       */
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

extern volatile sig_atomic_t	g_exit_status;

typedef struct s_cmd
{
	char						*line;
	char						**args;
	char						*cmd_path;
	int							in_fd;
	int							out_fd;
}								t_cmd;

// typedef enum s_nodetype
// {
// 	N_INVALID,
// 	N_UNDEFINED,
// 	N_OR,        // 1
// 	N_AND,       // 2
// 	N_PIPE,      // 3
// 	N_READ,      // 4
// 	N_HEREDOC,   // 5
// 	N_WRITE,     // 6
// 	N_APPEND,    // 7
// 	N_READWRITE, // 8
// 	N_HERESTR,   // 9
// 	N_SUBSHELL,  // 10
// 	N_CMD,       // 11
// }

typedef enum s_nodetype
{
	N_INVALID,
	N_UNDEFINED,
	N_OR,
	N_AND,
	N_PIPE,
	N_READ,
	N_HEREDOC,
	N_WRITE,
	N_APPEND,
	N_READWRITE,
	N_HERESTR,
	N_SUBSHELL,
	N_CMD,
}								t_nodetype;

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
	pid_t						pid_arr[4096];
	int							pid_index;
	char						*tmp_dir;
	int							heredoc[4096];
	char						**filename;
	int							abort;
	int							shlvl;
}								t_cmd_set;

typedef struct s_glob_match
{
	char						*glob;
	char						*file;
	char						*idxglob;
	char						*idxfile;
	char						*headglob;
	char						*headfile;
	int							quo[3];
	int							headqstate[3];
}								t_glob_match;

typedef struct s_token_params
{
	char						***a;
	int							*i;
	char						*ops;
	int							*q;
}								t_token_params;

/*
 * ========================================================
 *              UTILS (src/utils/)
 * ========================================================
 */

// free_exit.c
void							free_exit(t_cmd_set *p, int exit_code,
									char *msg);

// utils_array_helper.c
char							*ft_strjoin3(char *s1, char *s2, char *s3);
int								ft_arr_len(char **m);
char							**ft_dup_array(char **arr_ptr);
char							**ft_array_insert(char **in, char *newstr);

// utils_array.c
int								copy_until_replace(char **dst, char **src,
									int stop, int *pos);
int								copy_small_array(char **dst, char **small,
									int *pos);
void							copy_remaining(char **dst, char **src,
									int start, int *pos);
char							**ft_array_replace(char ***big, char **small,
									int n);

// utils_free_helper.c
t_list							*free_tmp_lst(t_list *cmds, char **args,
									char **temp);
void							close_fds_except_std(void);

// utils_free.c
void							free_cmd(t_cmd *cmd);
void							free_node(t_node *node);
void							free_lst(void *content);
void							free_array(char ***m);
void							free_all(char *s1, char *s2, char *s3,
									char *s4);

// utils.c
int								chrset_idx(const char *s, char *set);
int								char_index(const char *s, int c);

/*
 * ========================================================
 *         SIGNALS & ERRORS (src/signals_and_errors/)
 * ========================================================
 */

// handle_errors_helper.c
void							error_unclosed_quotes(void);
void							error_unexpected_token(char *cmd);
void							error_token_newline(void);
void							error_delim_heredoc(char *lim);
void							disable_ctrl_z(void);

// handle_errors.c
void							*put_err_helper(char *err_msg, char *cmd,
									t_cmd_set *p);
void							*put_err(char *err_msg, char *cmd, int err_code,
									t_cmd_set *p);
void							*put_err_cd(char *err_type, char *path,
									int code, t_cmd_set *p);
void							*put_err_syntax(char *err_msg, int err_code,
									t_cmd_set *p);

// handle_signals.c
extern volatile sig_atomic_t	g_exit_status;
void							set_signals(t_cmd_set *p);
void							signals_parent(int signal_code);
void							signals_child(int signal_code);
void							signals_heredoc(int signal_code);
int								rl_heredoc_hook(void);

/*
 * ========================================================
 *              PARSING (src/parsing/)
 * ========================================================
 */

// parse_node_types.c
t_nodetype						get_nodetype(char *str);
int								get_precedence(char *str);
t_node							*malloc_and_init_node(t_node *node);

// parse_node_utils.c
int								handle_subshell_error(char **args, int old_i,
									t_cmd_set *p);
int								check_subshell_forward_error(char **args, int i,
									int old_i, t_cmd_set *p);
int								skip_subshell_backward(char **args, int i,
									t_cmd_set *p);
int								skip_subshell_forward(char **args, int i,
									int l);
int								skip_subshell(char **args, int i, int direction,
									t_cmd_set *p);

// parse_node_find_op.c
int								process_subshell_in_find_op(char **args, int i,
									t_cmd_set *p);
int								find_lowest_precedence_op(char **args,
									t_cmd_set *p);
char							**trim_array(char **args, int i, int j);
void							print_tab(char **tab);
t_cmd							*parse_cmd(char *str, t_cmd_set *p);

// parse_node_checks.c
int								check_subshell_brackets(char **args, int len);
int								check_subshell_errors(char **args, int len,
									t_cmd_set *p);
int								check_subshell(t_node *node, t_cmd_set *p,
									char **args);
int								check_redirect_errors(char **args, int i,
									t_node *nodehead, t_cmd_set *p);
int								check_redirect_node(char **args, int i,
									t_node *nodehead, t_cmd_set *p);

// parse_node_handlers.c
t_node							*handle_redirect_node(char **args, int i,
									t_cmd_set *p, int index_base);
t_node							*handle_subshell_node(char **args, int i,
									t_cmd_set *p, int index_base);
t_node							*handle_binary_node(char **args, int i,
									t_cmd_set *p, int index_base);
t_node							*create_node_helper(char **args, t_cmd_set *p,
									int i, int index_base);
t_node							*create_node(char **args, t_cmd_set *p,
									int index_base);

// parse_nodes.c
void							*parse_nodes(char **args, t_cmd_set *p);

// expand_helper2.c
char							*get_home_dir(t_cmd_set *p);
char							*find_substitution(char first, char *var,
									t_cmd_set *p);
char							*expand_variable(char *str, int i, t_cmd_set *p,
									char *s[3]);
char							*expand_tilde(char *str, t_cmd_set *p);
int								get_tilde_expansion_length(char *str, int i);

// expand_helper.c
void							track_quotes(int *in_squote, int *in_dquote,
									char c);
char							*remove_char_at(char *str, size_t pos);
char							*put_char_in_squotes(char *str, size_t pos);
char							*remove_dollar_quote(char *str);

// expand.c
char							*handle_j_equal_one(char *left, char *home,
									char *right);
char							*handle_j_greater_than_one(char *left,
									char *home, char *right);
char							*build_expanded_string(char *left, char *home,
									char *right, int j);
char							*expand_tilde_at_position(char *str, int i,
									t_cmd_set *p, char *s[3]);
char							*var_expander(char *str, int *quotes,
									t_cmd_set *p);
// process_input_quote_utils.c
int								check_unclosed_quotes(const char *s);
int								handle_uquote(char **s, t_cmd_set *p);
int								is_in_quote(char c, int *q);
int								is_operator_char(char c, char *ops);
int								is_whitespace_in_quote(char c, int *q);

// process_input_token_count.c
void							handle_whitespace(char **input, int *q);
void							handle_non_operator(char **input, int *q,
									int *count, char *ops);
int								is_triple_redirect(char *input);
int								is_double_operator(char *input);
void							handle_operator(char **input, int *count);

// process_input_token_count2.c
int								count_tokens(char *input, char *ops);
char							*malloc_and_copy(char *index, int l);
char							*fill_token_tab_word(char *input, char *ops,
									int *q);
void							handle_word_token(char **input,
									t_token_params *params);
void							handle_operator_token(char **input, char ***a,
									int *i);

// process_input_token_fill.c
void							fill_token_tab(char *input, char ***a,
									char *ops, int k);
char							**init_filename_tab(void);
char							*strchr_outside_quotes(char *s, char c);
void							fill_filename_tab(char ***a, int token_count,
									t_cmd_set *p);

// process_input_token_rearrange.c
int								is_valid_join_condition(char ***a,
									char ***filename, int i, int token_counter);
void							handle_join_case(char ***a, int i);
int								is_valid_swap_condition(char ***a,
									char ***filename, int i, int token_counter);
void							handle_swap_case(char ***a, char ***filename,
									int i);
void							process_token(char ***a, char ***filename,
									int i, int token_counter);

// process_input_token_rearrange2.c
void							rearrange_tab(char ***a, char ***filename,
									int token_counter);
void							count_valid_entries(char ***a, char ***filename,
									int token_count, int *counts);
int								allocate_new_tab(char ***newtab,
									int total_size);
void							copy_and_cleanup_entries(char ***a,
									char ***filename, char **newtab,
									int token_count);
char							**realloc_tab(char ***a, char ***filename,
									int token_count);

// process_input_token_split.c
char							**split_by_op_ign_space_in_quote(char *input,
									char *ops, t_cmd_set *p);
int								check_delim_expand(char *lim);
int								handle_heredoc_case(t_cmd_set *p, int i,
									int *line_index);
void							set_final_exit_status(t_cmd_set *p);
int								read_heredocs(t_cmd_set *p, int *line_index,
									int syntax);

// process_input_syntax_check.c
int								check_first_token(char **tokens,
									int token_counter, t_cmd_set *p);
int								check_shlvl_bracket(char *token, int shlvl,
									int i, t_cmd_set *p);
int								check_token_pair(char **tokens, int i,
									t_cmd_set *p);
int								check_final_token(char **tokens, int i,
									int shlvl, t_cmd_set *p);
int								check_syntax(char **tokens, int token_counter,
									t_cmd_set *p);

// process_input_helper2.c
void							free_filename_tab(t_cmd_set *p);
int								init_process_vars(char **input,
									int *len_input_arr, int *line_index,
									int *syntax);
int								init_line_processing(char **input, t_cmd_set *p,
									int line_index, int *syntax);
char							**tokenize_input(char **input_line,
									t_cmd_set *p);
void							process_syntax_and_tokens(t_cmd_set *p,
									int *syntax, int *line_index);

// process_input.c
int								handle_heredoc_success(t_cmd_set *p,
									int syntax);
void							cleanup_process(t_cmd_set *p);
int								process_input_helper(t_cmd_set *p, int *syntax);
void							*process_input(char **input, t_cmd_set *p);

// process_input_handle_helper
void							construct_heredoc_string(char **s, int i[3],
									char *tmp[3]);
void							handle_heredoc_expansion(char **s, int i[3],
									int quo[2], char *tmp[3]);
void							process_heredoc(char **s, int i[3], int quo[2],
									char *tmp[3]);
void							upd_quo(int *quo, int c);
void							init_vars(int quo[3], int i[3], char **s,
									char *input);
// process_input_condition_check.c
int								is_triple_less_than(char *s, int pos,
									int quo[3]);
void							handle_double_less_than(char **s, int i[3],
									int quo[3], char *tmp[3]);
int								is_double_less_than(char *s, int pos,
									int quo[3]);

// heredoc_helper.c
int								get_here_str_helper(char **str, char *lim);
void							get_here_str_assign_str(char *str[3]);
char							*get_here_str(char *str[3], char *lim,
									t_cmd_set *p, int *line_index);
void							process_here(char *str[1], int fd[2],
									t_cmd_set *p, int expand);
void							init_quotes(int *quo);

// heredoc.c
char							*trim_all_quotes(char *str);
int								read_heredoc(char *str[3], char *lim,
									t_cmd_set *p);
void							trim_ext_squotes(char *lim);
int								read_heredoc_b(char *str[3], char *lim,
									t_cmd_set *p, int *line_index);
t_cmd							*in_fd_heredoc(t_cmd *node, char **args, int *i,
									t_cmd_set *p);

// parse_split_cmd.c
t_cmd							*init_cmd(void);
int								size_to_malloc(char *s, char *set, int count);
char							**add_to_array(char **tmpstr, char *s,
									char *set, int i[3]);
char							**ft_split_with_pipe_or_redir_char(
									char const *s, char *set);
char							**split_with_special_chars(char **args);

// process_input_helper.c
int								update_quotes_chk_curly_bracket(int *quotes,
									char ch, int i[3], char **s);
void							remove_curly_brackets(char **s, int i[3],
									int quotes[2], char *tmp[3]);
int								update_quotes_chk_heredoc(int *quo, char ch,
									int **i, char **s);
void							handle_curly_brackets(char **input);
void							handle_input(char **input, t_cmd_set *p);

// split_with_space.c
int								token_counter(const char *s, char *c, int i[2]);
char							**split_with_quotes(char **tmpstr, char *s,
									char *set, int i[3]);
char							**split_and_ignore_space_if_in_quote(char *s,
									char *set);

// syntax_checker.c
int								is_redir(char c);
char							*pipe_error(char **tok, int idx);
char							*redir_error(char **tok, int idx);
char							*is_invalid_syntax(char **tok, int *i);

// trim_quotes.c
int								check_quotes_get_len(char const *s1);
char							*remove_quotes(char const *s1, int squote,
									int dquote);

/*
 * ========================================================
 *         BUILT-IN FUNCTIONS (src/built-ins/)
 * ========================================================
 */

// builtin_cd_helper.c
void							cd_path_checks(char *path, t_cmd_set *p);
int								handle_special_cases(char ***av, char *home,
									char *oldpwd, t_cmd_set *p);

// builtin_cd.c
void							ft_chdir(char ***av, char *home, char *oldpwd,
									t_cmd_set *p);
void							check_home_pwd_oldpwd(t_cmd_set *p);
void							update_pwd_vars(t_cmd_set *p, char *prevpwd);
void							handle_oldpwd_output(char **cmd_args,
									char *oldpwd, t_cmd_set *p);
int								builtin_cd_b(t_cmd_set *p, char **cmd_args);

// builtin_checker.c
int								is_builtin(t_cmd *n);

// builtin_echo.c
int								c_count(char *s, char c);
int								builtin_echo_b(t_cmd *cmd);

// builtin_env.c
int								builtin_env_b(char **envp);

// builtin_exit.c
int								parse_digits(const char *str, int *i,
									unsigned long *n, int sign);
int								parse_strict_ll(const char *str,
									long long *out);
int								builtin_exit_b(t_cmd *cmd, t_cmd_set *p);
int								builtin_exit_child_b(t_cmd *cmd);

// builtin_export_helper.c
int								var_in_envp(char *arg, char **envp, int *idx);
int								validate_append_identifier(char *str, int pos);
int								validate_regular_identifier(char *str);
int								is_valid_identifier(char *str);

// builtin_export.c
void							sort_env(char **arr, int len);
void							print_export(char **envp);
void							handle_append_operation(char *arg,
									t_cmd_set *p);
void							process_export_arg(char *arg, t_cmd_set *p,
									int *status);
int								builtin_export_b(t_cmd_set *p, char **args);

// builtin_pwd.c
int								builtin_pwd_b(void);

// builtin_unset.c
int								builtin_unset_b(t_cmd_set *p, char **args);

// environement.c
int								find_env_var_index(char *var, char **envp);
char							*ft_getenv(char *var, char **envp);
char							**ft_setenv(char *var, char *value,
									char **envp);
void							handle_env_vars(char *str, int *i, int fd[2],
									t_cmd_set *p);

/*
 * ========================================================
 *              EXECUTION (src/execute/)
 * ========================================================
 */

// execute_fd_utils.c
void							dup2_and_close(int in_fd, int out_fd);
void							restore_std_fds(int orig_in, int orig_out);
void							handle_redirections(t_node *node);
void							setup_left_node_fds(t_node *node, int fd);
void							setup_left_node_output_fds(t_node *node,
									int fd);

// execute_fork_utils.c
void							fork_and_exec_helper(t_node *node, t_cmd *cmd,
									t_cmd_set *p);
int								fork_and_exec(t_node *node, t_cmd *cmd,
									t_cmd_set *p);
int								exec_node_par_builtins_b_helper_one(pid_t pid,
									t_cmd_set *p);
int								exec_node_par_builtins_b_helper_two(
									t_node *node,
									t_cmd *cmd, t_cmd_set *p, pid_t pid);
int								exec_node_par_builtins_b(t_node *node,
									t_cmd *cmd, t_cmd_set *p);

// execute_builtins.c
int								exec_builtins(t_node *node, t_cmd *cmd,
									t_cmd_set *p);
void							wait_and_set_status_helper(t_cmd_set *p,
									int last_status);
int								wait_and_set_status(t_cmd_set *p);
int								exec_cmd_node(t_node *node, t_cmd_set *p);
int								exec_node(t_node *node, t_cmd_set *p);

// execute_pipe_utils.c
int								setup_pipe_fds(t_node *node);
int								create_pipe(int *fd, t_cmd_set *p);
int								execute_left_pipe(t_node *node, int *fd,
									t_cmd_set *p);
int								execute_right_pipe(t_node *node, int *fd,
									t_cmd_set *p);
void							handle_pipe_completion(int status,
									t_cmd_set *p);

// execute_pipe.c
int								exec_pipe_node(t_node *node, t_cmd_set *p);
int								exec_node_and_or(t_node *node, t_cmd_set *p);
int								parent_is_redir(t_node *node, char c);
int								parent_is_redir_helper(char c, t_nodetype type,
									t_node *parent);

// execute_redirect_utils.c
int								handle_open_error(t_node *node, t_cmd_set *p);
int								handle_read_errors(t_cmd_set *p, int err);
int								handle_write_errors(t_cmd_set *p, int err);
int								handle_readwrite_errors(t_cmd_set *p, int err);
int								check_fd(int fd, t_node *node, t_cmd_set *p);

// execute_read.c
int								open_read_file(t_node *node, t_cmd_set *p);
int								execute_left_read_node(t_node *node,
									t_cmd_set *p, int fd);
int								exec_read_node(t_node *node, t_cmd_set *p);
int								handle_heredoc_left_node(t_node *node,
									t_cmd_set *p, int fd);
int								exec_heredoc_node(t_node *node, t_cmd_set *p);

// execute_write.c
int								open_write_file(t_node *node);
int								should_abort_execution(t_node *node,
									t_cmd_set *p);
int								execute_left_write_node(t_node *node,
									t_cmd_set *p, int fd);
int								exec_write_or_append_node(t_node *node,
									t_cmd_set *p);
int								open_readwrite_file(t_node *node, t_cmd_set *p);

// execute_readwrite.c
int								should_abort_readwrite_execution(t_node *node,
									t_cmd_set *p);
int								execute_left_readwrite_node(t_node *node,
									t_cmd_set *p, int fd);
int								exec_readwrite_node(t_node *node, t_cmd_set *p);
char							*create_herestr_filename(t_node *node,
									t_cmd_set *p);
int								create_and_write_herestr_file(char *filename,
									t_node *node, t_cmd_set *p);

// execute_herestr.c
int								open_herestr_file_for_reading(char *filename,
									t_node *node, t_cmd_set *p);
void							setup_left_node_herestr_fds(t_node *node,
									int fd);
int								execute_left_herestr_node(t_node *node,
									t_cmd_set *p, int fd);
int								exec_herestr_node(t_node *node, t_cmd_set *p);
int								should_remove_quote(char *file, int i,
									int q[2]);

// execute_quote_utils.c
void							remove_quote_and_shift(char *file, int *i,
									int init_len, int q[2]);
void							remove_token_brackets(char *file);
int								chk_inf_node_redir_perm_helper(t_node *node,
									int status);
int								chk_inf_node_redir_perm(t_node *node,
									t_cmd_set *p);
int								handle_ambiguous_redirect(char *file_orig,
									char **exp, t_cmd_set *p);

// execute_redirect.c
int								handle_redirect_type(t_node *node,
									t_cmd_set *p);
int								exec_redirect_node(t_node *node, t_cmd_set *p);
void							init_subshell(t_cmd_set *p, t_node *node);
void							check_for_heredoc_close_fd(t_node *node,
									t_cmd_set *p);
void							handle_parent_process(pid_t pid, t_node *node,
									t_cmd_set *p);

// execute_subshell.c
void							setup_left_node_readwrite_fds(t_node *node,
									int fd);
void							handle_child_process(t_node *node,
									t_cmd_set *p);
int								exec_subshell_node(t_node *node, t_cmd_set *p);

// glob_utils.c
char							*ft_strjoin_char_flag(char *str, char c,
									int quoted);
char							*ft_strjoin_char(char *str, char c);
char							*ft_strjoin_char_with_flag(char *str, char c,
									int quoted);
char							*preprocess_glob(char *glob, int *quo);
int								next_char(char **pattern, char *c, int *quoted);

// glob_matching.c
int								match_at_position(char *pattern, char *text);
int								find_glob(char *token, int *quo);
char							*advance_str_past_q(char *headglob, int *quo,
									int prev_q_state);
int								check_dot_file_pattern(char *original_glob);
char							*skip_stars(char *pattern);

// glob_matching2.c
char							*find_char_in_str(char *str, char target);
int								match_with_wildcard(char *pattern, char *text);
int								simple_glob_match(char *pattern, char *text);
int								match_glob(char *glob, char *file);
int								should_include_file(char *filename, char *glob);

// glob.c
char							**expand_glob(char *glob, t_cmd_set *p);
char							**glob_expander(char ***args, int *i,
									int quo[3], t_cmd_set *p);

// find_path_helper.c
char							*ensure_path(char *path);
void							handle_exec_err(t_cmd *n, DIR *dir,
									t_cmd_set *p);

// find_path.c
char							*build_cmd_path(const char *dir,
									const char *cmd);
char							*find_command(char **env_path, char *cmd,
									char *cmd_path);
void							process_checks(t_node *node, char *path,
									t_cmd_set *p, char ***s);
DIR								*cmd_checks(t_node *node, char ***s, char *path,
									t_cmd_set *p);
void							find_cmd_path(t_cmd_set *p, t_node *node,
									char *path);

/*
 * ========================================================
 *              		SRC (src/)
 * ========================================================
 */

// init2.c
void							init_pid_arr(t_cmd_set *p);
void							init_heredoc_arr(t_cmd_set *p);

// init.c
int								allocate_env_vars_if_envp_empty(t_cmd_set *p);
void							init_p_struct(t_cmd_set *p);
void							init_reset(t_cmd_set *p, char **envp);
void							init(t_cmd_set *p, char **envp, char **argv,
									int argc);

// main.c
char							*generate_prompt(void);
void							handle_interactive_input(t_cmd_set *p);
char							*read_full_input(void);
void							handle_non_interactive_input(t_cmd_set *p);

#endif
