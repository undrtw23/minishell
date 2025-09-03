# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ngaurama <ngaurama@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/22 18:50:02 by gkorzecz          #+#    #+#              #
#    Updated: 2025/09/03 22:43:34 by ngaurama         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

YELLOW = \033[38;5;226m
GREEN = \033[0;32m
ORANGE = \033[0;91m
BRIGHTORANGE = \033[38;5;208m
RED = \033[0;31m
BLUE = \033[38;5;21m
RESET = \033[0m

LIBFT_DIR			= libft/
LIBFT_NAME			= libft.a
LIBFT				= $(LIBFT_DIR)$(LIBFT_NAME)

FT_PRINTF_FD_DIR	= ft_printf_fd/
FT_PRINTF_FD_NAME	= libftprintf_fd.a
FT_PRINTF_FD		= $(FT_PRINTF_FD_DIR)$(FT_PRINTF_FD_NAME)

GET_NEXT_LINE_DIR	= get_next_line/
GET_NEXT_LINE_NAME	= get_next_line.a
GET_NEXT_LINE		= $(GET_NEXT_LINE_DIR)$(GET_NEXT_LINE_NAME)

SRC_DIR_F		= src
OBJ_DIR		= obj
OBJ_SUBDIRS = $(sort $(dir $(OBJ_FILES)))
#####
PARS			=	src/parsing/
EXEC			=	src/execution/
BI				=	src/built-ins/
SGE				=	src/signals_and_errors/
UTI				=	src/utils/

# SRC_FILES	=     $(PARS)heredoc.c $(PARS)heredoc_helper.c $(PARS)parse_node_types.c $(PARS)parse_node_utils.c \
# 					$(PARS)parse_node_find_op.c $(PARS)parse_node_checks.c $(PARS)parse_node_handlers.c \
# 					$(PARS)parse_nodes_main.c \
# 					$(PARS)process_input_quote_utils.c $(PARS)process_input_token_count.c \
# 					$(PARS)process_input_token_count2.c $(PARS)process_input_token_fill.c \
# 					$(PARS)process_input_token_rearrange.c $(PARS)process_input_token_rearrange2.c \
# 					$(PARS)process_input_token_split.c $(PARS)process_input_syntax_check.c \
# 					$(PARS)process_input_main.c $(PARS)process_input_main2.c \
# 					$(PARS)process_input_condition_check.c $(PARS)process_input_handle_helper.c \
# 					$(PARS)process_input_helper.c $(PARS)process_input_helper2.c \
# 					$(PARS)parse_split_cmd.c $(PARS)split_with_space.c \
# 					$(PARS)syntax_checker.c $(PARS)trim_quotes.c $(PARS)expand.c \
# 					$(PARS)expand_helper.c $(PARS)expand_helper_2.c $(PARS)create_fds.c \
# 					$(BI)builtin_export.c $(BI)builtin_export_helper.c \
# 					$(BI)builtin_cd.c $(BI)builtin_cd_helper.c  $(BI)builtin_checker.c $(BI)builtin_echo.c\
# 					$(BI)builtin_env.c  $(BI)builtin_exit.c\
# 					$(BI)builtin_pwd.c  $(BI)builtin_unset.c\
# 					$(BI)environement.c\
# 					$(EXEC)execute.c $(EXEC)find_path.c \
# 					$(EXEC)find_path_helper.c \
# 					$(EXEC)execute_fd_utils.c $(EXEC)execute_fork_utils.c $(EXEC)execute_builtins.c \
# 					$(EXEC)execute_pipe_utils.c $(EXEC)execute_pipe.c $(EXEC)execute_redirect_utils.c \
# 					$(EXEC)execute_read.c $(EXEC)execute_write.c $(EXEC)execute_readwrite.c \
# 					$(EXEC)execute_herestr.c $(EXEC)execute_quote_utils.c $(EXEC)execute_redirect.c \
# 					$(EXEC)execute_subshell.c $(EXEC)glob.c $(EXEC)glob_utils.c $(EXEC)glob_matching.c\
# 					$(EXEC)glob_matching2.c \
# 					$(SGE)handle_errors.c $(SGE)handle_errors_helper.c $(SGE)handle_signals.c\
# 					$(UTI)free_exit.c $(UTI)utils.c $(UTI)utils_array.c\
# 					$(UTI)utils_free.c \
# 					$(SRC_DIR_F)/init.c $(SRC_DIR_F)/init2.c $(SRC_DIR_F)/main.c

SRC_FILES	=     $(PARS)heredoc.c $(PARS)heredoc_helper.c $(PARS)parse_node_types.c $(PARS)parse_node_utils.c \
					$(PARS)parse_node_find_op.c $(PARS)parse_node_checks.c $(PARS)parse_node_handlers.c \
					$(PARS)parse_nodes.c \
					$(PARS)process_input_quote_utils.c $(PARS)process_input_token_count.c \
					$(PARS)process_input_token_count2.c $(PARS)process_input_token_fill.c \
					$(PARS)process_input_token_rearrange.c $(PARS)process_input_token_rearrange2.c \
					$(PARS)process_input_token_split.c $(PARS)process_input_syntax_check.c \
					$(PARS)process_input.c $(PARS)process_input_condition_check.c $(PARS)process_input_handle_helper.c \
					$(PARS)process_input_helper.c $(PARS)process_input_helper2.c \
					$(PARS)parse_split_cmd.c $(PARS)split_with_space.c \
					$(PARS)syntax_checker.c $(PARS)trim_quotes.c $(PARS)expand.c \
					$(PARS)expand_helper.c $(PARS)expand_helper_2.c \
					$(BI)builtin_export.c $(BI)builtin_export_helper.c \
					$(BI)builtin_cd.c $(BI)builtin_cd_helper.c  $(BI)builtin_checker.c $(BI)builtin_echo.c\
					$(BI)builtin_env.c  $(BI)builtin_exit.c\
					$(BI)builtin_pwd.c  $(BI)builtin_unset.c\
					$(BI)environement.c\
					$(EXEC)execute.c $(EXEC)find_path.c \
					$(EXEC)find_path_helper.c \
					$(EXEC)execute_fd_utils.c $(EXEC)execute_fork_utils.c $(EXEC)execute_builtins.c \
					$(EXEC)execute_pipe_utils.c $(EXEC)execute_pipe.c $(EXEC)execute_redirect_utils.c \
					$(EXEC)execute_read.c $(EXEC)execute_write.c $(EXEC)execute_readwrite.c \
					$(EXEC)execute_herestr.c $(EXEC)execute_quote_utils.c $(EXEC)execute_redirect.c \
					$(EXEC)execute_subshell.c $(EXEC)glob.c $(EXEC)glob_utils.c $(EXEC)glob_matching.c\
					$(EXEC)glob_matching2.c \
					$(SGE)handle_errors.c $(SGE)handle_errors_helper.c $(SGE)handle_signals.c\
					$(UTI)free_exit.c $(UTI)utils.c $(UTI)utils_array.c $(UTI)utils_array_helper.c\
					$(UTI)utils_free.c $(UTI)utils_free_helper.c \
					$(SRC_DIR_F)/init.c $(SRC_DIR_F)/init2.c $(SRC_DIR_F)/main.c
					
OBJ_FILES = $(SRC_FILES:$(SRC_DIR_F)/%.c=$(OBJ_DIR)/%.o)

######
#SRC_FILES	= $(wildcard $(SRC_DIR_F)/*.c)
#OBJ_FILES	= $(patsubst $(SRC_DIR_F)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

NAME		= minishell
CC			= cc
FLAGS		= -g -Wall -Werror -Wextra -ggdb -fsanitize=address,undefined
RM			= rm -rf
AR			= ar rcs
INCLUDE		= 	-I ./inc/\
				-I ./libft/\
				-I ./ft_printf_fd/\
				-I ./get_next_line/

all: $(LIBFT) $(FT_PRINTF_FD) $(GET_NEXT_LINE) $(OBJ_SUBDIRS) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR_F)/%.c
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(FLAGS) $(INCLUDE) -c $< -o $@

$(OBJ_SUBDIRS):
	@mkdir -p $@
	
$(LIBFT):
	@echo "$(YELLOW)Compiling libft...$(RESET)"
	@make bonus --no-print-directory -sC $(LIBFT_DIR)
	@echo "$(GREEN)libft ready!$(RESET)"

$(FT_PRINTF_FD):
	@echo "$(YELLOW)Compiling ft_printf_fd...$(RESET)"
	@make --no-print-directory -sC $(FT_PRINTF_FD_DIR)
	@echo "$(GREEN)ft_printf_fd ready!$(RESET)"

$(GET_NEXT_LINE):
	@echo "$(YELLOW)Compiling get_next_line...$(RESET)"
	@make --no-print-directory -sC $(GET_NEXT_LINE_DIR)
	@echo "$(GREEN)get_next_line ready!$(RESET)"

$(NAME): $(OBJ_FILES)
	@echo "$(YELLOW)Compiling minishell...$(RESET)"
	@$(CC) $(FLAGS) -o $(NAME) $(OBJ_FILES) $(LIBFT) $(FT_PRINTF_FD) $(GET_NEXT_LINE) -lreadline $(INCLUDE)
	@echo "$(GREEN)Minishell ready!$(RESET)"

clean:
	@echo "$(ORANGE)Cleaning object files...$(RESET)"
	@$(RM) $(OBJ_DIR)
	@make --no-print-directory clean -C $(LIBFT_DIR)
	@make --no-print-directory clean -C $(FT_PRINTF_FD_DIR)
	@make --no-print-directory clean -C $(GET_NEXT_LINE_DIR)

fclean: clean
	@echo "$(RED)Removing minishell...$(RESET)"
	@$(RM) $(NAME)
	@echo "$(RED)Removing libft...$(RESET)"
	@make --no-print-directory fclean -C $(LIBFT_DIR)
	@echo "$(RED)Removing ft_printf_fd...$(RESET)"
	@make --no-print-directory fclean -C $(FT_PRINTF_FD_DIR)
	@echo "$(RED)Removing get_next_line...$(RESET)"
	@make --no-print-directory fclean -C $(GET_NEXT_LINE_DIR)
	@echo "$(GREEN)Cleaning complete!$(RESET)"

re: fclean all
	@echo "$(BLUE)Clean and rebuild done!$(RESET)"

.PHONY: all re clean fclean
