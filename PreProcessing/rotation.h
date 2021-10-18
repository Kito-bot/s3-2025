#ifndef ROTATION_H
#define ROTATION_H 

#include <stdlib.h>
#include <err.h>
#include "SDL/SDL.h" 
#include "SDL/SDL_image.h"
#include "tools.h"
#include <math.h>

SDL_Surface* imgRotation(SDL_Surface *img, double angle);

#endif
