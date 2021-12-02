#ifndef IMAGE_HANDLING_H
#define IMAGE_HANDLING_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <err.h>

void init_sdl();

SDL_Surface* load_image(char *path);

SDL_Surface* display_image(SDL_Surface *img);

void wait_for_keypressed();

SDL_Surface* copy_image(SDL_Surface* img);

void save_images(SDL_Surface* image_source, size_t corner_x, size_t corner_y, size_t width, size_t height);
#endif
