#include "detection.h"
//#include "pixel_operations.h"
//#include "image_handling.h"
#include "../tools/tools.h"

SDL_Surface* detect_grid_edge(SDL_Surface* image_surface)
{
	size_t height = (size_t) image_surface -> h;
	size_t width = (size_t) image_surface -> w;

	SDL_Surface* image_copy = copy_image(image_surface);

	//size_t grid_edge_x;
	//size_t grid_edge_y;

	size_t x = 0;
	size_t y = 0;

	Uint32 pixel;
	Uint8 r, g, b;

	for (size_t i = 0; i < width; i++)
	{
		for (size_t j = 0; j < height; j++)
		{
			//grid_edge_x = i;
			//grid_edge_y = j;

			
			pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);

			while (r == 0 && g == 0 && b == 0 && j + y < height)
			{
				pixel = get_pixel(image_surface, i, j + y);
				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

				y += 1;
			}

			if (y > width / 5)
			{
				
				for (size_t k = j; k < y; k++)
				{
					pixel = get_pixel(image_surface, i, k);

					//SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
					pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
					put_pixel(image_copy, i,  k, pixel);

				}
				
			
				/*pixel = get_pixel(image_surface, i, j);

				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
				pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
				put_pixel(image_surface, i,  j, pixel);
				*/
				
			}
			
			else
			{
				for (size_t k = j; k < y; k++)
				{
					pixel = get_pixel(image_surface, i, k);

					//SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
					pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
					put_pixel(image_copy, i,  k, pixel);
				}
			}
			
			
			
			pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel, image_surface -> format, &r, &g, &b);

			while (r == 0 && g == 0 && b == 0 && i + x < width)
			{
				pixel = get_pixel(image_surface, i + x, j);
				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

				x += 1;
			}

			if (x > height / 5)
			{
				
				for (size_t k = i; k < x; k++)
				{
					pixel = get_pixel(image_surface, x, j);

					//SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
					pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
					put_pixel(image_copy, k,  j, pixel);

				}
				
				/*
					pixel = get_pixel(image_surface, i, j);

					SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
					pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
					put_pixel(image_surface, i,  j, pixel);
					*/
			}
			
			else
			{
				for (size_t k = i; k < x; k++)
				{
					pixel = get_pixel(image_surface, i, k);

					//SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
					pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
					put_pixel(image_copy, k,  j, pixel);
				}
			}
			



			/*
			pixel = get_pixel(image_surface, i, j);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

			while (r == 1 && g == 1 && b == 1 && i + x < height)
			{
				pixel = get_pixel(image_surface, i + x, j);
				SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
				x += 1;
			}

			if (x == y)
			{
				//r = 255;
				//pixel = SDL_MapRGB(image_surface->format,r,g,b);

				//put_pixel(image_surface, i, j, pixel);
				printf("%lu, %lu", i, j);
				return 1;
			}
			*/
			x = 0;
			y = 0;
		}

	}

	return image_copy;

}

SDL_Surface* detection2(SDL_Surface* img)
{
	double g_x[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

//	double g_y[3][3] = {{-1, -2, -1},{0, 0, 0},{1, 2, 1}};

	Uint32 pixel;

	Uint8 r, g, b;

	double sum_g_x = 0;
//	double sum_g_y = 0;

	double value = 100;

	SDL_Surface* copy = copy_image(img);

	for (size_t i = 1; i < (size_t) (img -> w); i++)
	{
		for (size_t j = 1; j < (size_t) (img -> h); j++)
		{

			for (int x = -1; x < 1; x++)
			{
				for (int y = -1; y < 1; y++)
				{
					pixel = get_pixel(img, i, j);
					SDL_GetRGB(pixel, img -> format, &r, &g, &b);

					if (r == 0)
					{
						value = 100;
					}
					else
					{
						value = 200;
					}

					sum_g_x += g_x[x + 1][y + 1]*value;
				}
			}

			if (sum_g_x < 0)
			{
				sum_g_x = -sum_g_x;
			}

			if (sum_g_x > 300)
			{
			//	pixel = SDL_MapRGB(img -> format, 255, 0, 0);
				put_pixel(copy, i, j, pixel);
			//	printf("1");
			}
			else
			{
				pixel = SDL_MapRGB(img -> format, 0, 0, 255);
				put_pixel(copy, i, j, pixel);
			}
			sum_g_x = 0;
		}
	}

	return copy;
}

void draw_empty_lines(SDL_Surface* img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	int nb_inter = 0;

	size_t i = 0;
	size_t j = 0;

	SDL_Surface* copy = copy_image(img);

	// detecte lignes vides et dessine en vert
	while (j < (size_t) (img -> w))
	{
		//pixel = get_pixel(img, j, 0);
		//SDL_GetRGB(pixel, img -> format, &r, &g, &b);
		
		while (i < (size_t) (img -> h))
		{
			pixel = get_pixel(img, j, i);

			SDL_GetRGB(pixel, img -> format, &r, &g, &b);

			
			if (r == 0)
			{
				nb_inter += 1;

			/*	
				while (i < ((size_t) (img -> h)) && r == 0)
				{
					pixel = get_pixel(img, j, i);
					SDL_GetRGB(pixel, img -> format, &r, &g, &b);
					i += 1;
				}
			*/	
			}

			i += 1;
		}

		if (nb_inter > 10)
		{
			// si + de 10 intersections avec pixels noirs : dessine lignei en vert
			size_t a = 0;

			pixel = SDL_MapRGB(img -> format, 0, 255, 0);

			while (a < (size_t) (img -> h))
			{
				put_pixel(copy, j, a, pixel);
				a += 1;
			}
		}

		nb_inter = 0;
		i = 0;

		j += 1;
 	}


	int border1 = 0;
	int border2 = 0;

	int max = 0;

	int max_border1 = 0;
	int max_border2 = 0;

	j = 0;
	i = (img -> h) / 2;

	while (j < (size_t) (img -> w))
	{
		pixel = get_pixel(copy, j, i);
		SDL_GetRGB(pixel, copy -> format, &r, &g, &b);

		while (r == 0 && g == 0 && b == 0)
		{
			pixel = get_pixel(copy, j, i);
			SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
			j += 1;
		}

		if (r == 0 && g == 255 && b == 0)
		{
			border1 = j;

			while (r == 0 && g == 255 && b == 0)
			{
				pixel = get_pixel(copy, j, i);
				SDL_GetRGB(pixel, copy -> format, &r, &g, &b);
				j += 1;
			}

			border2 = j - 1;

		}

		if (border2 - border1 > max)
		{
			max_border1 = border1;
			max_border2 = border2;

			max = border2 - border1;
		}

		j += 1;

	}

	if (max_border1 != 0 && max_border2 != 0)
	{
		for (int x = 0; x < (img -> h); x++)
		{
			pixel = SDL_MapRGB(img -> format, 255, 0, 0);
			put_pixel(img, max_border1, x, pixel);
			put_pixel(img, max_border2, x, pixel);
		}

	}

	printf("max border len = %i;    border1 == %i   border2 == %i\n", max, max_border1, max_border2);



	/*
	// detecte colonnes vides et dessine en bleu
	i = 0;
	j = 0;
	nb_inter = 0;

	while (i < (size_t) (img -> h))
	{
		//pixel = get_pixel(img, j, 0);
		//SDL_GetRGB(pixel, img -> format, &r, &g, &b);
		
		while (j < (size_t) (img -> w))
		{
			pixel = get_pixel(copy, j, i);

			SDL_GetRGB(pixel, copy -> format, &r, &g, &b);

			
			if (r == 0 && g == 0 && b == 0)
			{
				nb_inter += 1;
				

				while (j < (size_t) (img -> w) && (r == 0 && g == 0 && b == 0))
				{
					pixel = get_pixel(img, j, i);
					SDL_GetRGB(pixel, img -> format, &r, &g, &b);
					j += 1;
				}
				
			}

			j += 1;
		}

		if (nb_inter > 10)
		{
			size_t  a = 0;

			pixel = SDL_MapRGB(copy -> format, 0, 0, 255);

			while (a < (size_t) (img -> w))
			{
				put_pixel(img, a, i, pixel);
				a += 1;
			}
		}

		nb_inter = 0;
		j = 0;
		i += 1;
	}	
*/

	SDL_FreeSurface(copy);
	
}







int crop_vertical_blob(SDL_Surface* img)
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

			border2 = j - 1;
		}

		if (border2 - border1 > max)
		{
			max_border1 = border1;
			max_border2 = border2;
			max = border2 - border1;
		}

		j += 1;

	}

	// pas de lignes vides detectees: return FALSE (0)
	
	if (max_border1 == 0 && max_border2 == (img -> w))
	{
		return 0;
	}

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

	// crop successful return TRUE (1)

	return 1;


}

int crop_horizontal_blob(SDL_Surface* img)
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

			border2 = i - 1;
		}

		if (border2 - border1 > max)
		{
			max_border1 = border1;
			max_border2 = border2;
			max = border2 - border1;
		}

		i += 1;

	}

	// pas de lignes vides detectees: return FALSE (0)
	
	if (max_border1 == 0 && max_border2 == (img -> h))
	{
		return 0;
	}

	// crop
	
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

	// crop successful return TRUE (1)

	return 1;


}

