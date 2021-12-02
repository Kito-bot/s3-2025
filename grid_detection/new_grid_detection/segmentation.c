#include "segmentation.h"
//#include "pixel_operations.h"
//#include "image_handling.h"
#include "../tools/tools.h"

int crop_vertical_blob(SDL_Surface* img, int *border_1, int *border_2)
{
	Uint32 pixel;
	Uint8 r, g, b;
	int nb_inter = 0;
	int i = 0;
	int j = 0;

	/*-----------------------------------draw empty lines------------------------------------*/
	SDL_Surface* copy = copy_image(img);

	while (j < (img -> w))
	{
		while (i < (img -> h))
		{
			pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0)
			{
				nb_inter += 1;
			}

			i += 1;
		}

		if (nb_inter > 10)
		{
			int a = 0;
			pixel = SDL_MapRGB(img -> format, 0, 255, 0);
			while(a < (img -> h))
			{
				put_pixel(copy, j, a, pixel);
				a += 1;
			}
		}

		nb_inter = 0;
		i = 0;
		j += 1;
	}


	/*------------------------------------crop image----------------------------------------*/


	// detect green lines
	 
	int border1 = 0;
	int border2 = 0;

	int max = 0;
	int max_border1 = 0;
	int max_border2 = 0;

	j = 0;
	i = (img -> h) / 2;


	while (j < (img -> w))
	{
		pixel = get_pixel(copy, j, i);
		SDL_GetRGB(pixel, copy -> format, &r, &g, &b);

		while (r == 0 && g == 0 && b == 0 && (j < (img -> w)))
		{
			pixel = get_pixel(copy, j, i);
			SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
			j += 1;
		}

		if (r == 0 && g == 255 && b == 0)
		{
			border1 = j;

			while (r == 0 && g == 255 && b == 0 && (j < (img -> w)))
			{
				pixel = get_pixel(copy, j, i);
				SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
				j += 1;
			}

			border2 = j;
		}

		if (border2 - border1 > max)
		{
			max_border1 = border1;
			max_border2 = border2;
			max = border2 - border1;
		}

		j += 1;

	}



	*border_1 = max_border1;
	*border_2 = max_border2;


	// crop
	
	SDL_Surface* cropped_image = SDL_CreateRGBSurface(0, max_border2 - max_border1, (copy -> h), 32, 0, 0, 0, 0);
	
	for (int i = 0; i < (copy -> h); i++)
	{
		for (int j = max_border1; j < max_border2; j++)
		{
			pixel = get_pixel(img, j, i);
			put_pixel(cropped_image, j - max_border1, i, pixel);
		}
	}

	*img = *cropped_image;

	
	free(cropped_image);
	free(copy);

	return 1;


}

int crop_horizontal_blob(SDL_Surface* img, int *border_1, int *border_2)
{
	Uint32 pixel;
	Uint8 r, g, b;
	int nb_inter = 0;
	int i = 0;
	int j = 0;

	/*-----------------------------------draw empty lines------------------------------------*/
	SDL_Surface* copy = copy_image(img);

	while (i < (img -> h))
	{
		while (j < (img -> w))
		{
			pixel = get_pixel(img, j, i);
			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			if (r == 0)
			{
				nb_inter += 1;
			}

			j += 1;
		}

		if (nb_inter > 10)
		{
			int a = 0;
			pixel = SDL_MapRGB(img -> format, 0, 255, 0);
			while(a < (img -> w))
			{
				put_pixel(copy, a, i, pixel);
				a += 1;
			}
		}

		nb_inter = 0;
		j = 0;
		i += 1;
	}


	/*------------------------------------crop image----------------------------------------*/


	// detect green lines
	 
	int border1 = 0;
	int border2 = 0;

	int max = 0;
	int max_border1 = 0;
	int max_border2 = 0;

	i = 0;
	j = (img -> w) / 2;


	while (i < (img -> h))
	{
		pixel = get_pixel(copy, j, i);
		SDL_GetRGB(pixel, copy -> format, &r, &g, &b);

		while (r == 0 && g == 0 && b == 0 && (i < (img -> h)))
		{
			pixel = get_pixel(copy, j, i);
			SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
			i += 1;
		}

		if (r == 0 && g == 255 && b == 0)
		{
			border1 = i;

			while (r == 0 && g == 255 && b == 0 && (i < (img -> h)))
			{
				pixel = get_pixel(copy, j, i);
				SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
				i += 1;
			}
			
			border2 = i;
		}

		if (border2 - border1 > max)
		{
			max_border1 = border1;
			max_border2 = border2;
			max = border2 - border1;
		}

		i += 1;

	}

	*border_1 = max_border1;
	*border_2 = max_border2;


	SDL_Surface* cropped_image = SDL_CreateRGBSurface(0, (copy -> w), max_border2 - max_border1, 32, 0, 0, 0, 0);
	
	for (int i = max_border1; i < max_border2; i++)
	{
		for (int j = 0; j < (img -> w); j++)
		{
			pixel = get_pixel(img, j, i);
			put_pixel(cropped_image, j, i - max_border1, pixel);
		}
	}

	*img = *cropped_image;

	free(cropped_image);
	free(copy);
	// crop successful return TRUE (1)

	return 1;
}

void DetectBiggestBlob(SDL_Surface *image_surface, int *pos_x, int *pos_y)
{
	int b1, b2;
	int b3, b4;
	int b5, b6;
	int b7, b8;

	crop_vertical_blob(image_surface, &b1, &b2);
	*pos_y = b1;
	crop_horizontal_blob(image_surface, &b3, &b4);
	*pos_x = b3;

	crop_vertical_blob(image_surface, &b5, &b6);
	if (b5 != b1)
		*pos_y += b5;

	crop_horizontal_blob(image_surface, &b7, &b8);
	if (b7 != b3)
		*pos_x += b7;

	while ((b1 != b5) || (b2 != b6) || (b3 != b7) || (b4 != b8))
	{
		crop_vertical_blob(image_surface, &b1, &b2);
		*pos_y += b1;
		crop_horizontal_blob(image_surface, &b3, &b4);
		*pos_x += b3;

		crop_vertical_blob(image_surface, &b5, &b6);
		if (b5 != b1)
			*pos_y += b5;
		crop_horizontal_blob(image_surface, &b7, &b8);
		if (b7 != b3)
			*pos_x += b7;
	}
	
	SDL_SaveBMP(image_surface, "processing_steps/blob.bmp");

}
