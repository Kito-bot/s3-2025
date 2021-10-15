#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <err.h>

void init_sdl();
SDL_Surface* load_image(char *path);
SDL_Surface* display_image(SDL_Surface *img);
void wait_for_keypressed();
Uint32 get_pixel(SDL_Surface *surface, int x, int y);
void put_pixel(SDL_Surface *surface, int x, int y, Uint32 pixel);
void update_surface(SDL_Surface *screen, SDL_Surface *image);
SDL_Surface* copy_image(SDL_Surface *img);
int average_spacing(SDL_Surface *img, char isHorizontal);
int *img_to_int(SDL_Surface *img);

#endif
