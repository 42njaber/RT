SRC = window.c \
	  display_data.c \
	  images.c \
	  matrix.c \
	  matrix2.c \
	  math.c \
	  vector2.c \
	  vector3.c \
	  gen_type.c \
	  colors.c \
	  util.c \
	  hash_map.c \
	  hash_map2.c \
	  gnl/get_next_line.c \
	  create_clprogram.c \
	  init_opencl.c

NAME = libgxns.a
LFT = ../libft/libft.a
LPF = ../ft_printf/libftprintf.a
GLFW = ../glfw/src/libglfw3.a
FLAGS = -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

CC = gcc
RM = @rm -fv

DEFAULT	= "\x1b[39m"
RED		= "\x1b[31m"
YELLOW	= "\x1b[32m"
GREEN	= "\x1b[33m"
BLUE	= "\x1b[34m"
MAGENTA	= "\x1b[35m"
CYAN	= "\x1b[36m"
.PHONY: all, clean, force, fclean, re

all: $(NAME)

force:
	@true

$(LFT): force
	@$(MAKE) -C ../libft/

$(LPF): force
	@$(MAKE) -C ../ft_printf/

$(GLFW):
	sh -c "cd ../glfw/; cmake ."
	@$(MAKE) -C ../glfw/

$(NAME): $(MLX) $(LFT) $(LPF) $(GLFW) $(OBJ) ../inc/libgxns.h Makefile
	@rm -f $@
	ar -rc $@ $(OBJ)
	libtool -static -o $@ $@ $(LFT) $(LPF) $(GLFW)
	ranlib $@

%.o:%.c
	$(CC) $(FLAGS) -o $@ -c $< $(MACROS) -I../inc/

soft_clean:
	@echo "Cleaning library:"
	$(RM) $(NAME)
	@echo "Cleaning objects:"
	$(RM) $(OBJ)

clean:
	@echo 			"_________________"
	@echo 			"Cleaning objects:"$(RED)
	$(RM) $(OBJ)
	@echo $(DEFAULT)"_________________"
	$(MAKE) -C ../libft/ fclean
	$(MAKE) -C ../ft_printf/ fclean

fclean: clean
	@echo 			"_________________"
	@echo 			"Cleaning library:"$(RED)
	$(RM) $(NAME)
	@echo $(DEFAULT)"_________________"

re: fclean all

soft_re: soft_clean all
