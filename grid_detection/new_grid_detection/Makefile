# Simple SDL mini Makefile

CC=gcc -fsanitize=address

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl` -lSDL_image -lm -ldl

SRC = main.c ../tools/tools.c  image_operations.c segmentation.c queue.c sobel.c hough.c box_blur.c rotate.c
OBJ = $(SRC:.c=.o)
DEP = $(SRC:.c=.d)

all:  main

main: $(OBJ)


clean:
	${RM} *.o
	${RM} *.d
	${RM} main

# END
