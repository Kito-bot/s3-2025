#ifndef SOBEL_H_
#define SOBEL_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../image_processing/image_operations.h"
#include "../image_processing/segmentation.h"

double Convolution(SDL_Surface *image, int kernel[3][3], int row, int col);

void SobelEdgeDetection(SDL_Surface *in, SDL_Surface *out, double threshold);


#endif

