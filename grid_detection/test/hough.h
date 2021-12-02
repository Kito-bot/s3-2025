#ifndef HOUGH_H
#define HOUGH_H

// Include SDL2 libraries
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include <stdio.h>
#include <stdlib.h>

#include <math.h>

#include "pixel_operations.h"
#include "loader.h"

void Hough(SDL_Surface *image);
void drawLine(SDL_Surface *image, int x0, int y0, int x1, int y1, Uint32 pixel);

#endif