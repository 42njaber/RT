FILES = main.c \
		read_scene.c \
		read_xml.c \
		read_path.c \
		parse_nodes.c \
		parse.c \
		init.c \
		scenes.c \
		setup_view.c \
		switch_scene.c \
		gen_thumbnails.c \
		util.c \
		gen_matrices.c \
		loop_hook.c \
		key_hooks.c \
		mouse_hooks.c \
		hooks.c \
		load_opencl.c \
		display_opencl.c

CFLAGS = -Llibgxns -lgxns -framework OpenGL -framework OpenCL -framework IOKit -framework Cocoa -framework CoreVideo
FLAGS =  -Iinc/ -Wall -Wextra -Werror

KEYBOARD = QWERTY
MACROS = -D $(KEYBOARD)

SRC_DIR = src
SRC = $(FILES:%=$(SRC_DIR)/%)

NAME = rt

OBJ_DIR = obj
OBJ = $(FILES:%.c=$(OBJ_DIR)/%.o)

ifdef DEB
FLAGS += -fsanitize=address -g3
CFLAGS += -fsanitize=address -g3
endif

INC = inc/rt.h inc/types.h inc/libgxns.h inc/common.h
LIB = libgxns/libgxns.a
CC = gcc
RM = @rm -fv
TEST_FILE = test.out
.PHONY: all, test, clean, fclean, re, force

all: $(NAME)

force:
	@true

libgxns/libgxns.a: force
	@echo "Entering libgxns/"
	make -C libgxns/
	@echo "Leaving libgxns/"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC)
	@mkdir $(OBJ_DIR) &> /dev/null || true
	$(CC) $(FLAGS) -o $@ -c $< $(MACROS) 

$(NAME): $(LIB) $(OBJ) Makefile $(INC)
	$(CC) $(CFLAGS) -o $@ $(OBJ)

sclean:
	make -C libgxns/ soft_clean
	@echo "Cleaning target:"
	$(RM) $(NAME)
	@echo "Cleaning objects:"
	$(RM) $(OBJ)

clean:
	@echo "Cleaning objects:"
	$(MAKE) -C libgxns/ fclean
	$(RM) $(OBJ)

fclean: clean
	@echo "Cleaning target:"
	$(RM) $(NAME)

re: fclean all

sre: sclean all
