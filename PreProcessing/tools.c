#include "tools.h"
#include <stdio.h>

void init_sdl()
{
	//Initialize the SDL lib
	if(SDL_Init(SDL_INIT_VIDEO) == -1)
		errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
	//load the image on the path
	SDL_Surface *img;

	img = IMG_Load(path);
	if(!img)
		errx(3,"can't load %s: %s", path, IMG_GetError());

	return img;
}

SDL_Surface* display_image(SDL_Surface *img)
{
	//Display an SDL_Surface and return the window surface
	SDL_Surface *screen;

	screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_ANYFORMAT);
	if(screen == NULL)
	{
		errx(1, "Couldn't set %dx%d video mode: %s\n",
				img->w, img->h, SDL_GetError());
	}

	if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
		warnx("BlitSurface error: %s\n", SDL_GetError());

	SDL_UpdateRect(screen, 0, 0, img->w, img->h);

	return screen;
}

void wait_for_keypressed()
{
	//Pause the program until a key is pressed
	SDL_Event event;

	do
	{
		SDL_PollEvent(&event);
	} while(event.type != SDL_KEYDOWN);


	do
	{
		SDL_PollEvent(&event);
	}while(event.type != SDL_KEYUP);
}


Uint32 get_pixel(SDL_Surface *surface, int x, int y)
{
	//Get the pixel in the SDL_surface surface at the coordinates (x, y)
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
	switch(bpp) {
	case 1:
		return *p;
	
	case 2:
		return *(Uint16 *)p;
	
	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
		return p[0] << 16 | p[1] << 8 | p[2];
		else
		return p[0] | p[1] << 8 | p[2] << 16;
	
	case 4:
		return *(Uint32 *)p;
	
	default:
		return 0;
	}
	
	}
	
	void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
	{
		
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	
	switch(bpp) {
	case 1:
		*p = pixel;
		break;
	
	case 2:
		*(Uint16 *)p = pixel;
		break;
	
	case 3:
		if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
		p[0] = (pixel >> 16) & 0xff;
		p[1] = (pixel >> 8) & 0xff;
		p[2] = pixel & 0xff;
		} else {
		p[0] = pixel & 0xff;
		p[1] = (pixel >> 8) & 0xff;
		p[2] = (pixel >> 16) & 0xff;
		}
		break;
	
	case 4:
		*(Uint32 *)p = pixel;
		break;
	}
}
	
	
	
void update_surface(SDL_Surface *screen, SDL_Surface *image)
{
	//Refresh the window screen with the SDL_Surface image
	if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
		warnx("BlitSurface error: %s\n", SDL_GetError());
	SDL_UpdateRect(screen, 0, 0, image->w, image->h);
}

SDL_Surface* copy_image(SDL_Surface *img)
{
	//Copy a SDL_Surface into an other and return it
	Uint32 pixel;
	SDL_Surface* copy;
	copy = SDL_CreateRGBSurface(SDL_ANYFORMAT,
						img->w,
						img->h,
						img->format->BitsPerPixel,
						img->format->Rmask,
						img->format->Gmask,
						img->format->Bmask,
						img->format->Amask);

	
	for(int x = 0; x < img->w; x++)
	{
		for(int y = 0; y < img->h; y++)
		{
			pixel = get_pixel(img, x, y);
			put_pixel(copy, x, y, pixel);
		}
	}

	return copy;
}

int average_spacing(SDL_Surface *img, char isHorizontal)
{
	//Compute the average spacing between two black pixel in an SDL_Surface
	int h, w;

	if(isHorizontal)
	{
		h = img->h;
		w = img->w;
	}
	else
	{
		w = img->h;
		h = img->w;
	}
	Uint32 pixel;
	Uint8 r;
	Uint8 g;
  	Uint8 b;
	int totalWPixel = 0;
	int count = 0;
	int nbSpaces = 0;

	for(int j = 0; j < h; j++)
	{
		for(int i = 0; i < w; i++)
		{
			if(isHorizontal)
			{
				pixel = get_pixel(img, i, j);
			}
			else
			{
				pixel = get_pixel(img, j, i);
			}

			SDL_GetRGB(pixel, img -> format, &r, &g, &b);
			if(r == 255 && g == 255 && b == 255)
				count++;
			else
			{
				if(count)
				{
					nbSpaces++;
					totalWPixel += count;
					count = 0;
				}
			}
		}

	}

	return(totalWPixel / nbSpaces);

}

int *img_to_int(SDL_Surface *img)
{
    int *bits = malloc(sizeof(int) * 256);
    for(int j = 0; j < img->h; j++)
    {
	for(int i = 0; i < img->w; i++)
	{
	    Uint32 pixel;
	    Uint8 r, g, b;

	    pixel = get_pixel(img, i, j);
	    SDL_GetRGB(pixel, img->format, &r, &g, &b);
	    if(!r && !g && !b)
	    {
		bits[j * img->w + i] = 1;
	    }
	    else
	    {
		bits[j * img->w + i] = 0;
	    }

	}
    }

    return bits;
}
