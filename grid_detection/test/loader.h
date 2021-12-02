#ifndef LOADER_H
#define LOADER_H

// Include SDL2 libraries
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

//void clean_resources(SDL_Window *w, SDL_Renderer *r, SDL_Texture *t);
SDL_Surface *load_img(char *path);
int loader(char * file);

#endif