# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/26 14:32:38 by vneelix           #+#    #+#              #
#    Updated: 2020/07/31 11:43:58 by vneelix          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SPATH = ./src/

OPATH = ./obj/

IPATH = ./inc/

SRC = $(wildcard $(SPATH)*.c)
	
INC = $(wildcard $(IPATH)*.h)

OBJ = $(SRC:$(SPATH)%.c=$(OPATH)%.o)

NAME = RT

CRFLAGS = -Wall -Wextra -Werror -O2 -I./inc -c

CDFLAGS = -g -O0 -I./inc -c

SDL2 = -lSDL2 -lm -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal

LFLAGS = -lm -Llib -lft $(SDL2) -framework OpenCL -o $(NAME)

MODE = RELEASE

.PHONY: all clean fclean re

$(OPATH)%.o: $(SPATH)%.c $(INC)
ifeq ($(MODE), debug)
	gcc $(CDFLAGS) $< -o $@
else
	gcc $(CRFLAGS) $< -o $@
endif

$(NAME): $(OBJ)
	gcc -Wall -Wextra -Werror $^ $(LFLAGS)

all: $(NAME)

debug:
	$(MAKE) MODE=debug

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
