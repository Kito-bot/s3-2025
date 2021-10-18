#ifndef PIXEL_OPERATIONS_H_
#define PIXEL_OPERATIONS_H_

#include <stdlib.h>
#include "SDL/SDL.h" //<SDL.h>

Uint8 *pixel_ref(SDL_Surface *surf, unsigned x, unsigned y);

#endif