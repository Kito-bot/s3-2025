#ifndef SCALING_H
#define SCALING_H

#include "SDL/SDL.h"

float lerp(float a, float b, float t);
float blerp(float a, float b, float c, float d, float tx, float ty);
SDL_Surface* scale(SDL_Surface *img, int w, int h);

#endif
