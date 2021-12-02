#include "rotate.h"
/*
void Rotate(SDL_Surface *img, double angle) // angle in degrees
{
	int new_w;
	int new_h;
	double theta = angle;
	
	if (theta < 90)
	{
		new_w = (img -> w * cos((theta * M_PI) / 180)) + (img -> h * sin((theta * M_PI) / 180));
		new_h = (img -> w * sin((theta * M_PI) / 180)) + (img -> h * cos((theta * M_PI) / 180));
	}

	else if (theta == 90)
	{
		new_w = img -> h;
		new_h = img -> w;
	}

	else // theta > 90
	{
		int tmp_h = img -> w;
		int tmp_w = img -> h;

		theta = angle - 90;

		new_w = (tmp_w * cos((theta * M_PI) / 180)) + (tmp_h * sin((theta * M_PI) / 180));
		new_h = (tmp_w * sin((theta * M_PI) / 180)) + (tmp_h * cos((theta * M_PI) / 180));
	}

	SDL_Surface *new_img = SDL_CreateRGBSurface(SDL_HWSURFACE, new_w, new_h, 32, 0, 0, 0, 0);

	int half_w = new_img -> w / 2;
	int half_h = new_img -> h / 2;

	Uint32 pixel;

	for (int i = 0; i < new_h; i++)
	{
		for (int j = 0; j < new_w; j++)
		{
			int x = (i - half_h) * cos((theta * M_PI) / 180) + (j - half_w) * sin((theta * M_PI) / 180);
			int y = (j - half_w) * cos((theta * M_PI) / 180) + (i - half_h) * sin((theta * M_PI) / 180);

			if (x >= 0 && x < img -> h && y >= 0 && y < img -> w)
			{	
				pixel = get_pixel(img, y, x);
				put_pixel(new_img, j, i, pixel);
			}
			else
			{
				pixel = SDL_MapRGB(new_img -> format, 255, 255, 255);
				put_pixel(new_img, j, i, pixel);
			}
		}
	}

	*img = *new_img;
	free(new_img);
	
}
*/
/*
void Rotate(SDL_Surface *img, double angle) // angle in degrees
{
	
}
*/
