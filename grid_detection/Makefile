CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image`

all: display

display: display.o pixel_operations.o

display.o: tools.h pixel_operations.h

pixel_operations.o: pixel_operations.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale
