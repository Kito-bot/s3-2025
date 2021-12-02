#ifndef IMAGE_OPERATIONS_H
#define IMAGE_OPERATIONS_H

#include <stdlib.h>
#include <SDL/SDL.h>
#include <err.h>
#include "../tools/tools.h"
#include <math.h>
#include "segmentation.h"
#include "box_blur.h"
#include "hough.h"
#include "sobel.h"

void grayscale(SDL_Surface* image_surface);
Uint8 otsuThreshold(SDL_Surface* image_surface);
void binarize(SDL_Surface* image_surface);
void increaseLux(SDL_Surface* image_surface, int lux);
void dilate(SDL_Surface* image_surfacen, int dil);
void Rotate(SDL_Surface* img, double angle);

SDL_Surface* NoiseReduction(SDL_Surface* img);

void PreProcessing(SDL_Surface *image_surface);

void DrawGrid(SDL_Surface *image_surface, SDL_Surface *img_copy, int px, int py);

void Draw(SDL_Surface *img, SDL_Surface *number, int px, int py);

void DrawImages(SDL_Surface *img, int px, int py, int h, int w, char *grid_input_path, char *grid_output_path);

void EraseVerticalLines(SDL_Surface *img);

void EraseHorizontalLines(SDL_Surface *img);

void EraseLines(SDL_Surface *img);

void SaveResultImage(SDL_Surface *processed_img, SDL_Surface *copy_img, int grid_pos_x, int grid_pos_y, char *grid_input_path, char *grid_output_path, char *image_result_path);

void Segmentation(SDL_Surface *image_surface, int *grid_pos_x, int *grid_pos_y, double sobel_threshold, double hough_threshold);

SDL_Surface *ResizeSurface(SDL_Surface *img, int new_height, int new_width);

void LineDetection(SDL_Surface *image_to_process, double sobel_threshold, double hough_threshold, char *line_color);

void EraseImageBorders(SDL_Surface *img, double wd);
#endif
