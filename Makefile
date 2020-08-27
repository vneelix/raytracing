# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vneelix <vneelix@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/26 14:32:38 by vneelix           #+#    #+#              #
#    Updated: 2020/08/10 11:57:44 by vneelix          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SPATH = ./src/

OPATH = ./obj/

IPATH = ./inc/

LIBFTPATH = ./libft/
LIBFT = ./libft/libft.a
LIBFTINC = ./libft/inc/

SRC = $(SPATH)control.c $(SPATH)file.c $(SPATH)file1.c $(SPATH)file2.c $(SPATH)func.c $(SPATH)func2.c $(SPATH)genhemisphere.c $(SPATH)get_item.c $(SPATH)get_item1.c \
	$(SPATH)get_scene.c $(SPATH)get_scene1.c $(SPATH)keyboard_control.c $(SPATH)main.c $(SPATH)menu.c $(SPATH)mouse_control.c $(SPATH)opencl.c $(SPATH)opencl_function.c \
	$(SPATH)opencl_kernel_func.c $(SPATH)opencl_kernel_func2.c $(SPATH)openclbuilder.c $(SPATH)prepare_item.c $(SPATH)read_texture.c $(SPATH)read_texture2.c $(SPATH)sdl_func.c
	
INC = $(IPATH)openclbuilder.h $(IPATH)rt.h

OBJ = $(SRC:$(SPATH)%.c=$(OPATH)%.o)

NAME = RT

CRFLAGS = -Wall -Wextra -Werror -O2 -I$(IPATH) -c

CDFLAGS = -g -O0 -I$(IPATH) -c

SDL2 = -lSDL2 -lSDL2_image -lm -liconv -Wl,-framework,CoreAudio -Wl,-framework,AudioToolbox -Wl,-framework,ForceFeedback -lobjc -Wl,-framework,CoreVideo -Wl,-framework,Cocoa -Wl,-framework,Carbon -Wl,-framework,IOKit -Wl,-weak_framework,QuartzCore -Wl,-weak_framework,Metal

LFLAGS = -lm -framework OpenCL -L$(LIBFTPATH) -lft -Llib $(SDL2) -o $(NAME)

MODE = RELEASE

.PHONY: all clean fclean re

$(OPATH)%.o: $(SPATH)%.c $(INC)
ifeq ($(MODE), debug)
	gcc $(CDFLAGS) -I$(LIBFTINC) $< -o $@
else
	gcc $(CRFLAGS) -I$(LIBFTINC) $< -o $@
endif

all: libft_make $(NAME)

libft_make:
	$(MAKE) -C $(LIBFTPATH)

$(NAME): $(OBJ) $(LIBFT)
	gcc -Wall -Wextra -Werror $(OBJ) $(LFLAGS)

debug:
	$(MAKE) MODE=debug

clean:
	$(MAKE) fclean -C $(LIBFTPATH)
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all
