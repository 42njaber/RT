FILES = main.c \
		display.c \
		read_cfg.c \
		parse_objs.c \
		cfg_default.c \
		init.c \
		hit_equations.c \
		gen_matricies.c \
		normal.c \
		loop_hook.c \
		hooks.c

ALT_FILE = load_opencl.c \
		   display_opencl.c \
		   buf_handler.c

CFLAGS = -Llibgxns -lgxns -framework OpenGL -framework AppKit

ifndef KEYBOARD
KEYBOARD = AZERTY
endif

MACROS = -D $(KEYBOARD)

ifndef OPENCL
OPENCL = TRUE
endif

ifeq ($(OPENCL), TRUE)
MACROS += -D OPENCL
FILES += load_opencl.c display_opencl.c buf_handler.c
CFLAGS += -framework OpenCL
else
endif

SRC_DIR = src
SRC = $(FILES:%=$(SRC_DIR)/%)

NAME = rt

OBJ_DIR = obj
OBJ = $(FILES:%.c=$(OBJ_DIR)/%.o)

LIB = libgxns/libgxns.a
FLAGS = -Wall -Wextra -Werror -Iinc/ 
CC = gcc
RM = @rm -fv
TEST_FILE = test.out
.PHONY: all, test, clean, fclean, re, force

all: $(NAME)

force:
	@true

libgxns/libgxns.a: force
	make -C libgxns/ OPENCL=$(OPENCL)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c inc/rt.h
	@mkdir $(OBJ_DIR) &> /dev/null || true
	$(CC) $(FLAGS) -o $@ -c $< $(MACROS) 

$(NAME): $(LIB) $(OBJ) Makefile inc/rt.h
	$(CC) $(CFLAGS) -o $@ $(OBJ)

soft_clean:
	make -C libgxns/ soft_clean
	@echo "Cleaning target:"
	$(RM) $(NAME)
	@echo "Cleaning objects:"
	$(RM) $(OBJ) $(ALT_FILE:%=$(OBJ_DIR)/%.o)

clean:
	@echo "Cleaning objects:"
	$(MAKE) -C libgxns/ fclean
	$(RM) $(OBJ) $(ALT_FILE:%=$(OBJ_DIR)/%.o)

fclean: clean
	@echo "Cleaning target:"
	$(RM) $(NAME)

re: fclean all

soft_re: soft_clean all
