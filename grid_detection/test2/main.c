#include "SDL/SDL.h"
#include "SDL/SDL_image"
#include <math.h>
//#include "hough_transform.h"
#include "operations.h"
//#include "rotate.h"
#include <sys/stat.h>
#include "hough.h"
#include "tools.h"
#define pi 3.14159265359


int main(void){

	SDL_Surface * img = load_image("sudoku-grid.png");

	edge_detection("sudoku-grid.bmp");


	return 0;
}