CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image`

all: main

main: hough.o tools.o
flood_filling: flood_filling.o tools.o image_split.o

hough.o: tools.h hough.h

tools.o: tools.h

image_split.o: image_split.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} display
	${RM} flood_filling