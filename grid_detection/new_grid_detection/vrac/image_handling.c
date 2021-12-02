//#include "image_handling.h"
//#include "pixel_operations.h"

#include "../tools/tools.h"

void init_sdl()
{
    // Init only the video part.
    // If it fails, die with an error message.
    if(SDL_Init(SDL_INIT_VIDEO) == -1)
        errx(1,"Could not initialize SDL: %s.\n", SDL_GetError());
}


SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;

    // Load an image using SDL_image with format detection.
    // If it fails, die with an error message.
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());

    return img;
}


SDL_Surface* display_image(SDL_Surface *img)
{
    SDL_Surface *screen;

    // Set the window to the same size as the image
    screen = SDL_SetVideoMode(img->w, img->h, 0, SDL_SWSURFACE|SDL_ANYFORMAT);
    if (screen == NULL)
    {
        // error management
        errx(1, "Couldn't set %dx%d video mode: %s\n",
                img->w, img->h, SDL_GetError());
    }

    // Blit onto the screen surface
    if(SDL_BlitSurface(img, NULL, screen, NULL) < 0)
        warnx("BlitSurface error: %s\n", SDL_GetError());

    // Update the screen
    SDL_UpdateRect(screen, 0, 0, img->w, img->h);

    // return the screen for further uses
    return screen;
}

void wait_for_keypressed()
{
    SDL_Event event;

    // Wait for a key to be down.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYDOWN);

    // Wait for a key to be up.
    do
    {
        SDL_PollEvent(&event);
    } while(event.type != SDL_KEYUP);
}

void SDL_FreeSurface(SDL_Surface *surface);

SDL_Surface* copy_image(SDL_Surface *img)
{
	Uint32 pixel;
	SDL_Surface* copy;

	copy = SDL_CreateRGBSurface(SDL_HWSURFACE,
			img->w,
			img->h,
			img->format->BitsPerPixel, 0, 0, 0, 0);
	for (int i = 0; i < img->w; i++)
	{
		for (int j = 0; j < img -> h; j++)
		{
			pixel = get_pixel(img, i, j);
			put_pixel(copy, i, j, pixel);
		}
	}

	return copy;
}



void save_image(SDL_Surface* image_source, size_t x, size_t y, int width, int height, char* path)
{
	SDL_Surface* img = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);

	SDL_Surface* scaled_img = SDL_CreateRGBSurface(0, 28, 28, 32, 0, 0, 0, 0);

	Uint32 pixel;

	for (size_t i = 0; i < (size_t) (img -> w); i++)
	{
		for (size_t j = 0; j < (size_t) (img -> h); j++)
		{
			pixel = get_pixel(image_source, x + i, y + j);
		       put_pixel(img, i, j, pixel);	
		}
	}

	SDL_SoftStretch(img, NULL, scaled_img, NULL);

	SDL_SaveBMP(scaled_img, path);
}

void save_images(SDL_Surface* image_source, size_t corner_x, size_t corner_y, size_t width, size_t height)
{

	// remplacer 50 par taille des cases, une fois detectees


	char name[20]; // nom du fichier, empty array
	char path[10] = {'d', 'i', 'g', 'i', 't', 's', '/', ' ', ' ', '\0'}; //fichier ou stocker images
	int a = 1; // numero du fichier (nom)

	for (size_t i = corner_y; i < (size_t) (corner_y + height - (height / 9)); i += (int) (height) / 9)
	{
		for (size_t j = corner_x; j < (size_t) (corner_x + width - (width / 9)); j += (int) (width) / 9)
		{
			sprintf(name, "%i", a);// convertir en string le nombre

			// creation du nom de l'image
			if (a < 10)
			{
				path[7] = name[0];
				path[8] = '\0';
			}
			else
			{
				path[7] = name[0];
				path[8] = name[1];
			}
			a += 1; // incremente le numero de l'image
			save_image(image_source, j, i, width / 9, height / 9, path);
		}
	}

}

