#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "noise.h"
#include "rotation.h"
#include "scaling.h"
#include "to_binary.h"
#include "tools.h"

#include "SDL/SDL.h"
#include "SDL/SDL_image.h"

int main(void){
	SDL_Surface* pic;
	SDL_Surface* screen;
	pic = load_image("../grid_detection/sudoku-grid.png");
	screen = display_image(pic);
	wait_for_keypressed();

	//pic = scale(pic,250,250);
	screen = display_image(pic);

	wait_for_keypressed();

	//pic = imgRotation(pic,-90.0f); //rotate to right (clockwise)
	screen = display_image(pic);

	wait_for_keypressed();

	/*for(size_t i=0;i<100;++i){
		noiseReduction(pic); //for clear pics lower threshhold by 30%
		screen = display_image(pic);
	}*/
	
	//wait_for_keypressed();

	grayscale(pic);
	screen = display_image(pic);

	wait_for_keypressed();

	to_binary(pic); //for clear pics lower threshhold by 30%
	screen = display_image(pic);
	
	wait_for_keypressed();

	SDL_FreeSurface(pic);
	SDL_FreeSurface(screen);

	return 0;
	
}