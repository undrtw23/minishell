# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gkorzecz <gkorzec@student.42.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/04/21 13:34:41 by gkorzecz          #+#    #+#              #
#    Updated: 2025/04/21 13:37:10 by gkorzecz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFILES	= get_next_line.c get_next_line_utils.c
SRCS	= $(CFILES)

OBJ_DIR	= obj
OFILES	= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

NAME	= get_next_line.a

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
