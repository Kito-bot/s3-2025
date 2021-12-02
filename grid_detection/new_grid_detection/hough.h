#ifndef HOUGH_H
#define HOUGH_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "../image_processing/image_operations.h"
#include "../image_processing/segmentation.h"
#include <err.h>

void HoughTransform(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, char *line_color);

double HoughTransformAngleDetection(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, char *line_color);

void DrawLine(SDL_Surface *img, int x1, int y1, int x2, int y2);

void DrawLine_v2(SDL_Surface *img, int x1, int y1, int x2, int y2);

void DrawLine_v3(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color);

void Flip(SDL_Surface *img);

#endif
