#include "box_blur.h"

void BoxBlur(SDL_Surface *img)
{
	Uint32 pixel;
	Uint8 r, g, b;

	SDL_Surface *image = copy_image(img);

	float average_red, average_blue, average_green;

	for (int i = 0; i < image -> h; i++)
	{
		for (int j = 0; j < image -> w; j++)
		{
			average_red = 0;
			average_green = 0;
			average_blue = 0;

			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					if (j + y >= 0 && j + y < image -> w && i + x < image -> h && i + x >= 0)
					{
						pixel = get_pixel(image, j + y, i + x);
						SDL_GetRGB(pixel, image -> format, &r, &g, &b);

						average_red += r;
						average_green += g;
						average_blue += b;
					}
				}
			}

			average_red = average_red / 9;
			average_blue = average_blue / 9;
			average_green = average_green / 9;
			

			pixel = SDL_MapRGB(image -> format, average_red, average_green, average_blue);
			put_pixel(img, j, i, pixel);
		}
	}

	free(image);
}
