# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/08/08 15:56:45 by vneelix           #+#    #+#              #
#    Updated: 2020/08/08 16:29:15 by vneelix          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SRC_P = ./src/

INC_P = ./inc/

OBJ_P = ./obj/

SRC = 	$(SRC_P)ft_atoi.c		$(SRC_P)ft_memalloc.c		$(SRC_P)ft_putendl_fd.c		$(SRC_P)ft_strncat.c \
		$(SRC_P)ft_bzero.c		$(SRC_P)ft_memccpy.c		$(SRC_P)ft_putnbr.c		$(SRC_P)ft_strncmp.c \
		$(SRC_P)ft_directory.c		$(SRC_P)ft_memchr.c		$(SRC_P)ft_putnbr_fd.c		$(SRC_P)ft_strncpy.c \
		$(SRC_P)ft_get_directory.c	$(SRC_P)ft_memcmp.c		$(SRC_P)ft_putstr.c		$(SRC_P)ft_strndup.c \
		$(SRC_P)ft_getfile.c		$(SRC_P)ft_memcpy.c		$(SRC_P)ft_putstr_fd.c		$(SRC_P)ft_strnstr.c \
		$(SRC_P)ft_isalnum.c		$(SRC_P)ft_memdel.c		$(SRC_P)ft_strcat.c		$(SRC_P)ft_strsplit.c \
		$(SRC_P)ft_isalpha.c		$(SRC_P)ft_memmove.c		$(SRC_P)ft_strchr.c		$(SRC_P)ft_strstr.c \
		$(SRC_P)ft_isascii.c		$(SRC_P)ft_memset.c		$(SRC_P)ft_strcmp.c		$(SRC_P)ft_tolower.c \
		$(SRC_P)ft_isdigit.c		$(SRC_P)ft_nptr_del.c		$(SRC_P)ft_strcpy.c		$(SRC_P)ft_toupper.c \
		$(SRC_P)ft_isprint.c		$(SRC_P)ft_putchar.c		$(SRC_P)ft_strdup.c		$(SRC_P)ft_ultoa.c \
		$(SRC_P)ft_itoa.c		$(SRC_P)ft_putchar_fd.c		$(SRC_P)ft_strjoin.c \
		$(SRC_P)ft_ltoa.c		$(SRC_P)ft_putendl.c		$(SRC_P)ft_strlen.c

INC = $(INC_P)libft.h

OBJ = $(SRC:$(SRC_P)%.c=$(OBJ_P)%.o)

NAME = libft.a

CC = gcc

CFLAGS = -Wall -Wextra -Werror -I$(INC_P) -c

.PHONY: all clean fclean re

$(OBJ_P)%.o: $(SRC_P)%.c $(INC)
	$(CC) $(CFLAGS) $< -o $@

$(NAME): $(OBJ)
	ar rc $(NAME) $^
	ranlib $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
