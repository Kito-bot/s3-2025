CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wall -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image`

<<<<<<< HEAD
all: display

display: display.o

display.o: tools.h
=======
all: flood_filling

flood_filling: flood_filling.o tools.o

flood_filling.o: tools.h

tools.o: tools.h
>>>>>>> d0d1a33 (reorganization of the git)

clean:
	${RM} *.o
	${RM} *.d
	${RM} display
	${RM} grayscale
