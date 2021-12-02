#include <stdio.h>
#include <stdlib.h>
//#include "pixel_operations.h"
#include "image_operations.h"
//#include "image_handling.h"
#include "segmentation.h"
#include "../tools/tools.h"
#include "sobel.h"
#include "hough.h"
#include "box_blur.h"

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <string.h>
#include "queue.h"
#include "rotate.h"
/*
int main(int argc, char** argv)
{
	if (argc > 3)
	{
		return EXIT_FAILURE;
	}

	SDL_Surface* image_surface;
	SDL_Surface* img_copy;
	SDL_Surface* screen_surface;

//	/+--------------------------------------Init SDL-----------------------------------------+
	
	init_sdl();

	image_surface = load_image(argv[1]);
	img_copy = copy_image(image_surface);

	screen_surface = display_image(image_surface);
	update_surface(screen_surface, image_surface);

//	wait_for_keypressed();


//	/+------------------------------------Preprocessing--------------------------------------+

	PreProcessing(image_surface);
	
	int px, py; // grid postion

	Segmentation(image_surface, &px, &py);
	SaveResultImage(image_surface, img_copy, px, py, "grid", "grid.result", "result.bmp");

	// Rotation

//	if (argc == 3)
//	{
//	double angle = strtoul(argv[2], NULL, 10);
//	image_surface = rotate(image_surface, angle);
//	screen_surface = display_image(image_surface);
//	update_surface(screen_surface, image_surface);
//	wait_for_keypressed();
//	}


//	/+----------------------------------Sauvegarde des images---------------------------------+

	
	wait_for_keypressed();

//	SDL_FreeSurface(img_copy);
	SDL_FreeSurface(image_surface);
	SDL_FreeSurface(screen_surface);


	return EXIT_SUCCESS;

}
*/



int main(int argc, char **argv)
{
	if (argc > 5)
	{
		return EXIT_FAILURE;
	}

	if (!strcmp(argv[1], "-sobel"))
	{
		SDL_Surface *in = load_image(argv[2]);
		SDL_Surface *out = SDL_CreateRGBSurface(0, in -> w, in -> h, 32, 0, 0, 0, 0);
		PreProcessing(in);
		SobelEdgeDetection(in, out, strtoul(argv[3], NULL, 10));
		SDL_SaveBMP(in, "processing_steps/in.bmp");
		SDL_SaveBMP(out, "processing_steps/sobel.bmp");
	}

	if (!strcmp(argv[1], "-hough"))
	{
		SDL_Surface *in = load_image(argv[2]);
		PreProcessing(in);
		int px, py;
		Segmentation(in, &px, &py, strtoul(argv[3], NULL, 10), strtoul(argv[4], NULL, 10));
		SDL_SaveBMP(in, "processing_steps/hough.bmp");
	}

	if (!strcmp(argv[1], "-detect"))
	{
		SDL_Surface *in = load_image(argv[2]);
		SDL_Surface *in_copy = copy_image(in);
		PreProcessing(in);
		int px, py; // coordinates of the upper left corner of the grid
		DetectBiggestBlob(in, &px, &py);
		DrawGrid(in, in_copy, px, py);
		SDL_SaveBMP(in_copy, "processing_steps/in_copy.bmp");
	}

	if (!strcmp(argv[1], "-angle"))
	{
		SDL_Surface *in = load_image(argv[2]);
		SDL_Surface *in_copy = copy_image(in);
		SDL_Surface *edge_image = SDL_CreateRGBSurface(0, in -> w, in -> h, 32, 0, 0, 0, 0);
		PreProcessing(in_copy);
		SobelEdgeDetection(in_copy, edge_image, strtoul(argv[3], NULL, 10));
		double tilt_angle = HoughTransformAngleDetection(edge_image, in_copy, 180, 180, strtoul(argv[4], NULL, 10), "blue");
		Rotate(in, tilt_angle);
		printf("\nAngle : %f\n", tilt_angle);
		SDL_SaveBMP(in, "processing_steps/hough.bmp");
		free(edge_image);

	}

	if (!strcmp(argv[1], "-rotate"))
	{
		double angle = strtoul(argv[3], NULL, 10);
		SDL_Surface *in = load_image(argv[2]);
		Rotate(in, angle);
		SDL_SaveBMP(in, "processing_steps/rotated.bmp");
	}
	return EXIT_SUCCESS;
}










