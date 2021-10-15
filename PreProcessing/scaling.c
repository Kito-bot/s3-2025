#include "scaling.h"
#include "tools.h"

float lerp(float a, float b, float t)
{
	return	a + (b - a) * t;
}

float blerp(float a, float b, float c, float d, float tx, float ty)
{
	float x = lerp(lerp(a, b, tx), lerp(c, d, tx), ty);
	return x;
}

SDL_Surface* scale(SDL_Surface *img, int w, int h)
{
	SDL_Surface *simg = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w,
			h,
			img->format->BitsPerPixel,
			img->format->Rmask,
			img->format->Gmask,
			img->format->Bmask,
			img->format->Amask
			);

	for(int x = 0; x < w; x++)
	{
		for(int y = 0; y < h; y++)
		{
			float gx = ((float)x) / ((float)w) * (((float)img->w) - 1);
			float gy = ((float)y) / ((float)h) * (((float)img->h) - 1);
			int gxi = (int)gx;
			int gyi = (int)gy;

			Uint32 pixel1 = get_pixel(img, gxi, gyi);
			Uint32 pixel2 = get_pixel(img, gxi + 1, gyi);
			Uint32 pixel3 = get_pixel(img, gxi, gyi + 1);
			Uint32 pixel4 = get_pixel(img, gxi + 1, gyi + 1);

			Uint8 r1, r2, r3, r4;
			Uint8 g1, g2, g3, g4;
			Uint8 b1, b2, b3, b4;
			
			SDL_GetRGB(pixel1, img->format, &r1, &g1, &b1);
			SDL_GetRGB(pixel2, img->format, &r2, &g2, &b2);
			SDL_GetRGB(pixel3, img->format, &r3, &g3, &b3);
			SDL_GetRGB(pixel4, img->format, &r4, &g4, &b4);

			int r = (int)blerp(
					(float)r1, 
					(float)r2, 
					(float)r3, 
					(float)r4,
					gx - (float)gxi,
					gy - (float)gyi);
			int g = (int)blerp(
					(float)g1, 
					(float)g2, 
					(float)g3, 
					(float)g4,
					gx - (float)gxi,
					gy - (float)gyi);
			int b = (int)blerp(
					(float)b1, 
					(float)b2, 
					(float)b3, 
					(float)b4,
					gx - (float)gxi,
					gy - (float)gyi);

			Uint32 pixel = SDL_MapRGB(img->format, r, g, b);
			put_pixel(simg, x, y, pixel);
		}
	}

	return simg;
}
