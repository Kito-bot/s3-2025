CC=gcc
CPPFLAGS= 
CFLAGS= -Wall -Wextra -Werror -std=c99 -O2
LDFLAGS=
LDLIBS= -lm -ldl

#${OBJ}= weight_save_load.o main.o matrice_operations.o "../trivia/animation.o"

all: main

main: weight_save_load.o main.o matrice_operations.o ../trivia/animation.o

#matrice_operations: matrice_operations.o
#neural_network_operations: neural_network_operations.o
#main: matrice_operations.o neural_network_operations.o

#matrice_operations.o: matrice_operations.h
#neural_network_operations.o: neural_network_operations.h
main.o: weight_save_load.h
weight_save_load.o: weight_save_load.h
#matrice_operations.o: matrice_operations.h

../trivia/animation.o: ../trivia/animation.h

clean:
	${RM} *.o
	${RM} *.d
	${RM} main
	${RM} xor_*
