CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image` -lm

all: main

main: hough.o loader.o main.o pixel_operations.o

hough.o: hough.h

loader.o: loader.h

main.o: main.h

pixel_operations.o: pixel_operations.h

clean:
		${RM} *.o
		${RM} *.d
		${RM} main