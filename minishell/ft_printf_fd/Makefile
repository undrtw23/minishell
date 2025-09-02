# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gkorzec <gkorzecz@student.42.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/27 17:19:24 by gkorzec           #+#    #+#              #
#    Updated: 2025/04/21 13:23:35 by gkorzecz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFILES	= ft_printchar_fd.c ft_printstr_fd.c ft_printf_fd.c ft_printhex_fd.c \
		  ft_printnbr_fd.c ft_printunsi_fd.c ft_printptr_fd.c

SRCS	= $(CFILES)

OBJ_DIR	= obj
OFILES	= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

NAME	= libftprintf_fd.a

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OFILES)
	@ar -rcs $(NAME) $(OFILES)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
