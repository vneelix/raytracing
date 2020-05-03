# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/26 14:32:38 by vneelix           #+#    #+#              #
#    Updated: 2020/05/02 03:38:32 by vneelix          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SPATH = ./src/

OPATH = ./obj/

IPATH = ./inc/

SRC = $(wildcard $(SPATH)*.c)
	
INC = $(wildcard $(IPATH)*.h)

OBJ = $(SRC:$(SPATH)%.c=$(OPATH)%.o)

NAME = RTv1

CRFLAGS = -Wall -Wextra -Werror -O2 -I./inc -c

CDFLAGS = -g -O0 -I./inc -c

LFLAGS = -lm -lSDL2 -lOpenCL -L./lib -lft -o $(NAME)

MODE = RELEASE

.PHONY: all clean fclean re

$(OPATH)%.o: $(SPATH)%.c $(INC)
ifeq ($(MODE), debug)
	clang $(CDFLAGS) $< -o $@
else
	clang $(CRFLAGS) $< -o $@
endif

$(NAME): $(OBJ)
	clang -Wall -Wextra -Werror $^ $(LFLAGS)

all: $(NAME)

debug:
	$(MAKE) MODE=debug

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
