CC=gcc
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image` -lm

all: main

main: solver_tools.o

solver_tools.o: solver_tools.h


clean:
		${RM} *.o
		${RM} *.d
		${RM} main
		${RM} *.result