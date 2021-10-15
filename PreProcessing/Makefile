CC=gcc
CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image` -lm

all: main

main: noise.o rotation.o to_binary.o tools.o scaling.o

noise.o: noise.h

rotation.o: rotation.h

tools.o: tools.h

scaling.o: scaling.h

to_binary.o: to_binary.h 
clean:
		${RM} *.o
		${RM} *.d
		${RM} main