CC=gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS= -Wextra -Werror -std=c99 -O3
LDFLAGS=
LDLIBS= `pkg-config --libs sdl SDL_image` -lm

all: main

main: binary.o edge_extraction.o file_io.o hough_transform.o max_filtering.o quick_sort.o

binary.o: settings.h binary.h

edge_extraction.o:settings.h edge_extraction.h

file_io.o: file_io.h settings.h

hough_transform.o: hough_transform.h settings.h 

max_filtering.o:max_filtering.h hough_transform.h settings.h

quick_sort.o:hough_transform.h quick_sort.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main