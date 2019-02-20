# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: llelievr <llelievr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/11/07 14:49:27 by llelievr          #+#    #+#              #
#    Updated: 2019/02/19 21:47:07 by llelievr         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME=wolf3d
SDL=${HOME}/.brew/Cellar/sdl2/2.0.9
SDL_IMG=${HOME}/.brew/Cellar/sdl2_image/2.0.4
SDL_TTF=${HOME}/.brew/Cellar/sdl2_ttf/2.0.14
CFLAGS=-g -Wall -flto -O3 -ffast-math -Wextra -Iinclude -I$(SDL_IMG)/include/SDL2 -I$(SDL_TTF)/include/SDL2 -I$(SDL)/include/SDL2
CC=gcc

include src.mk

OBJS=$(addprefix $(OBJDIR),$(SRC:.c=.o))

# ft library
FT		=./libft/
FT_LIB	=$(addprefix $(FT), libft.a)
FT_INC	=-I ./libft/includes
FT_LNK	=-L ./libft
FT_TASK =

SRCDIR	=./srcs/
INCDIR	=./includes/
OBJDIR	=./objs/

all: $(FT_LIB) $(NAME)

$(OBJS): Makefile src.mk

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(FT_INC) -I $(INCDIR) -o $@ -c $<

$(FT_LIB):
	make -j4 -C $(FT)

$(NAME): $(OBJS) $(FT_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(FT_LNK) -L$(SDL)/lib -lSDL2 -L$(SDL_IMG)/lib -lSDL2_image -L$(SDL_TTF)/lib -lSDL2_ttf -lm $(OBJS) $(FT_LIB)

clean:
	make -C $(FT) clean
	rm -f $(OBJS)

fclean: clean
	rm -rf $(NAME)
	make -C $(FT) fclean

re: fclean $(NAME)

get_files:
	$(shell find srcs -type f | sed 's/srcs\///g' | sed 's/^/SRC+=/' > src.mk)

.PHONY: clean fclean re all
