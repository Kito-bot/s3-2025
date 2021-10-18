#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "tools.h"
#include "to_binary.h"

void  to_binary(SDL_Surface *image_surface) 
{
	//Compute the binary version of an image
	int width = image_surface->w;
	int height = image_surface->h;
	
	for (int x = 0; x < width; ++x) {
		for (int y = 0; y < height; ++y) {
		Uint8 r,g,b;
		SDL_GetRGB(get_pixel(image_surface, x, y),
			image_surface->format, &r, &g, &b);
		r = g = b = (0.3*r + 0.59*g + 0.11*b) > 180 ? 255 : 0;
		Uint32 pixel = SDL_MapRGB(image_surface->format, r, g, b); 
		put_pixel(image_surface, x, y, pixel);
        }
    }

}

void grayscale(SDL_Surface *img)
{
	//Compute the grayscale of an image
	for(int x = 0; x < img->w; x++)
	{
	     	for(int y = 0; y < img->h; y++)
		{
		     	Uint32 pixel = get_pixel(img, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, img->format, &r, &g, &b);
			Uint8 average = 0.3 * r + 0.59 * g + 0.11 * b;
			pixel = SDL_MapRGB(img->format, average, average, average);
			put_pixel(img, x, y, pixel);
		}
	}
}
