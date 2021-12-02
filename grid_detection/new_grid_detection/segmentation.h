#ifndef SEGMENTATION_H
#define SEGMENTATION_H

#include <SDL/SDL.h>

int crop_vertical_blob(SDL_Surface* img, int *border_1, int *border_2);

int crop_horizontal_blob(SDL_Surface* img, int *border_1, int *border_2);

void DetectBiggestBlob(SDL_Surface *image_surface, int *pos_x, int *pos_y);
#endif
