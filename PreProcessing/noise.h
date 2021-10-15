#ifndef NOISE_H
#define NOISE_H

#include <stdlib.h>
#include <SDL.h>

void array_swap(int array[], size_t i, size_t j);
void array_select_sort(int array[], size_t len);
void noiseReduction(SDL_Surface *img);


#endif