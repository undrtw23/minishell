# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gkorzec <gkorzecz@student.42.fr>           +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/27 17:19:24 by gkorzec           #+#    #+#              #
#    Updated: 2025/05/01 21:14:31 by gkorzecz         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

CFILES	= ft_atoi.c ft_bzero.c ft_isalnum.c ft_isalpha.c ft_isascii.c ft_isdigit.c \
		  ft_isprint.c ft_itoa.c ft_memset.c ft_putchar_fd.c ft_putendl_fd.c \
		  ft_putstr_fd.c ft_strchr.c ft_strdup.c ft_strjoin.c ft_strlcpy.c \
		  ft_strlen.c ft_strncmp.c ft_strrchr.c ft_tolower.c ft_toupper.c \
		  ft_memcpy.c ft_memmove.c ft_strlcat.c ft_memchr.c ft_memcmp.c \
		  ft_strnstr.c ft_calloc.c ft_substr.c ft_strtrim.c ft_split.c \
		  ft_strmapi.c ft_striteri.c ft_putnbr_fd.c ft_strcmp.c

CBONUS	= ft_lstnew_bonus.c ft_lstsize_bonus.c ft_lstiter_bonus.c \
		  ft_lstclear_bonus.c ft_lstdelone_bonus.c ft_lstlast_bonus.c \
		  ft_lstadd_front_bonus.c ft_lstadd_back_bonus.c

SRCS	= $(CFILES)
SRCSALL	= $(CFILES) $(CBONUS)

OBJ_DIR	= obj
OFILES	= $(addprefix $(OBJ_DIR)/, $(SRCS:.c=.o))
OFILESALL = $(addprefix $(OBJ_DIR)/, $(SRCSALL:.c=.o))

CC		= cc
CFLAGS	= -Wall -Wextra -Werror

NAME	= libft.a

all: $(OBJ_DIR) $(NAME)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OFILES)
	@ar -rcs $(NAME) $(OFILES)

bonus: $(OBJ_DIR) $(OFILESALL)
	@ar -rcs $(NAME) $(OFILESALL)

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re bonus
