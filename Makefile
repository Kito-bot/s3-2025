CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image`

all: display

display: display.o

display.o: tools.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale
