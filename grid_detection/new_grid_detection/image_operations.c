#include "image_operations.h"

void grayscale(SDL_Surface* image_surface)
{
	Uint32 pixel;
	Uint8 r, g, b;
	Uint8 average;

	for (int i = 0; i < image_surface->h; i++)
	{
		for (int j = 0; j < image_surface->w; j++)
		{
			pixel = get_pixel(image_surface, j, i);
			SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
			average = 0.3 * r + 0.59 * g + 0.11 * b;
			pixel = SDL_MapRGB(image_surface->format, average, average, average);
			put_pixel(image_surface, j, i, pixel);	
		}
	}
}

Uint8 otsuThreshold(SDL_Surface* image_surface)
{
	unsigned long histo[256];
	Uint8 r, g, b;

 	for (int x = 0; x < 256; x++)
		histo[x] = 0;

 	for (int i = 0; i < image_surface->h; i++)
 	{
	 	for (int j = 0; j < image_surface->w; j++)
	 	{	
		 	Uint32 pixel = get_pixel(image_surface, j, i);
		       	SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);

		 	histo[r] += 1;
	 	}
 	}

 	unsigned long sum = 0, sum1 = 0, w1 = 0, w2 = 0;
 	unsigned long m1, m2;
 	float between = 0, max = 0;
 	Uint8 threshold1 = 0, threshold2 = 0;
 	unsigned long total = image_surface->h * image_surface->w;

 	for (int i = 0; i < 256; i++)
	 	sum += i * histo[i];

 	for (int i = 0; i < 256; i++)
 	{
	 	w1 += histo[i];
	 	if (w1 == 0)
		 	continue;

	 	w2 = total - w1;
	 	if (w2 == 0)
		 	break;

	 	sum1 += i * histo[i];
	 	m1 = sum1 / w1;
	 	m2 = (sum - sum1) / w2;
	 	between = w1 * w2 * (m1 - m2) * (m1 - m2);
	 	if (between >= max)
	 	{
		 	threshold1 = i;
		 	if (between > max)
			 	threshold2 = i;

		 	max = between;
	 	}
 	}

 	Uint8 threshold = (threshold1 + threshold2) / 2;
 	return threshold;
}

void binarize(SDL_Surface* image_surface)
{
	Uint8 r, g, b;
	Uint32 pixel;
	Uint8 threshold = otsuThreshold(image_surface);

	for (int i = 0; i < image_surface->h; i++)
	{
		for (int j = 0; j < image_surface->w; j++)
		{
			pixel = get_pixel(image_surface, j, i);
                        SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);

			if (r > threshold)
			{
				pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
                        	put_pixel(image_surface, j, i, pixel);
			}

			else
			{
				pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
                                put_pixel(image_surface, j, i, pixel);
			}
		}
	}
}


void dilate(SDL_Surface* image_surface, int dil)
{
	int board[image_surface->h][image_surface->w];

	for (int i = 0; i < image_surface->h; i++)
	{
		for (int j = 0; j < image_surface->w; j++)
			board[i][j] = 0;
	}

	Uint32 pixel;
	Uint8 r, g, b;

	for (int di = 0; di < image_surface->h; di++)
  	{
	  	for (int dj = 0; dj < image_surface->w; dj++)
	  	{
		  	pixel = get_pixel(image_surface, dj, di);
                  	SDL_GetRGB(pixel,image_surface->format,&r,&g,&b);

		  	if (r == 0)
		  	{
			  	int dk = 0;

			  	while (dk <= dil)
			  	{
				  	if (di + dk < image_surface->h)
				  	{
					  	if (dj + dk < image_surface->w)
					  	{
						  	board[di + dk][dj + dk] = 1;
					  	}

					  	if (dj - dk >= 0)
                                          	{
                                                  	board[di + dk][dj - dk] = 1;
                                          	}
				  	}

				  	if (di - dk >= 0)
                                  	{
                                          	if (dj + dk < image_surface->w)
                                          	{
                                                  	board[di - dk][dj + dk] = 1;
                                          	}

                                          	if (dj - dk >= 0)
                                          	{
                                                  	board[di - dk][dj - dk] = 1;
                                          	}
                                  	}

				  	dk += 1;
			  	}
		  	}
	  	}
  	}

	Uint32 newpixel;

  	for (int i = 0; i < image_surface->h; i++)
  	{
	  	for (int j = 0; j < image_surface->w; j++)
	  	{
		  	if (board[i][j] == 1)
		  	{	
				newpixel = SDL_MapRGB(image_surface->format,0,0,0);
                        	put_pixel(image_surface, j, i,newpixel);
		  	}
	  	}
  	}
}

void increaseLux(SDL_Surface* image_surface, int lux)
{
	Uint32 pixel;
	Uint8 r, g, b;

	for (int i = 0; i < image_surface->h; i++)
	{
		for (int j = 0; j < image_surface->w; j++)
		{
			pixel = get_pixel(image_surface, j, i);
                        SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);

			if (r > 175)
			{
                        	pixel = SDL_MapRGB(image_surface->format, r - lux, r - lux, r - lux);
                        	put_pixel(image_surface, j, i, pixel);
			}
		}
	}
}

void GetNewSize(SDL_Surface *img, double angle, int *h, int *w) // rip, doesnt work..
{
	angle = fmod(angle, 180) ;

	if (angle < 0)
	{
		angle = 180 + angle;
	}
	if (angle >= 90)
	{
		int tmp = *h;
		*h = *w;
		*w = tmp;
		angle = angle - 90;
	}
	if (angle == 0)
	{
		return ;
	}

	double rad = angle * M_PI / 180;

	*w = (img -> w) * cos(rad) + (img -> h) * sin(rad);
	*h = (img -> w) * sin(rad) + (img -> h) * cos(rad);
}

void Rotate(SDL_Surface* img, double angle)
{
	//int n_w, n_h;

	//GetNewSize(img, angle, &n_h, &n_w);

	SDL_Surface* new_img = SDL_CreateRGBSurface(SDL_HWSURFACE, img -> w, img -> h, 32, 0, 0, 0, 0);
	int center_x = img -> w / 2;
	int center_y = img -> h / 2;
	
	angle = angle * M_PI / 180;
	
	for (int i = 0; i < img -> w; i++)
	{
		for (int j = 0; j < img -> h; j++)
		{
			int x = (int) ((i - center_x) * cos(angle) - (j - center_y) * sin(angle) + center_x);
			int y = (int) ((i - center_x) * sin(angle) + (j - center_y) * cos(angle) + center_y);
			
			if (x >= 0 && y >= 0 && x < img -> w && y < img -> h)
			{
				Uint32 pixel = get_pixel(img, x, y);
				put_pixel(new_img, i, j, pixel);
			}
			else
			{
				Uint32 pixel = SDL_MapRGB(img -> format, 255, 255, 255);
				put_pixel(new_img, i, j, pixel);
			}
		}
	}
	
	*img = *new_img;
	free(new_img);
}

SDL_Surface* NoiseReduction(SDL_Surface* img)
{
	int neighbours[] = {-1, -1, -1, 1, 1, -1, 0, -1, -1, 0, 1, 0, 0, 1, 1, 1};

	Uint32 pixel;

	Uint8 r, g, b;

	SDL_Surface* result = SDL_CreateRGBSurface(SDL_HWSURFACE, img -> w, img -> h, 32, 0, 0, 0, 0);

	for (int i = 0; i < (result -> w); i++)
	{
		for (int j = 0; j < (result -> h); j++)
		{
			int x = 0;
			int y = 1;

			int nbPixelOn = 0;

			while (y < 16)
			{
				pixel = get_pixel(img, i, j);

				SDL_GetRGB(pixel, img->format, &r, &g, &b);
				if (((i + neighbours[x] < (img -> w))
				&& (i + neighbours[x] >= 0))
				&& ((j + neighbours[y] >= 0)
				&& (j + neighbours[y] < (img -> h))) && (r == 0))
				{
					pixel = get_pixel(img, i + neighbours[x], j + neighbours[y]);
					SDL_GetRGB(pixel, img->format, &r, &g, &b);
					if (r == 0 && g == 0 && b == 0)
					{
						nbPixelOn += 1;
					}
				}
				x += 2;
				y += 2;	 
			}

			if (nbPixelOn < 3)
			{
				pixel = SDL_MapRGB(img->format, 255, 255, 255);
				put_pixel(result, i, j, pixel);
			}
			/*
			else
			{
				pixel = SDL_MapRGB(img -> format, 0, 0, 0);
				put_pixel(result, i, j, pixel);
			}
			*/
		}
	}

	return result;
}

void PreProcessing(SDL_Surface *image_surface)
{
//	SDL_SaveBMP(image_surface, "processing_steps/brightness.bmp");
	grayscale(image_surface);
//	SDL_SaveBMP(image_surface, "processing_steps/grayscale.bmp");
	BoxBlur(image_surface);
	BoxBlur(image_surface);
//	SDL_SaveBMP(image_surface, "processing_steps/blurred.bmp");
	binarize(image_surface);
	*image_surface = *NoiseReduction(image_surface);
//	SDL_SaveBMP(image_surface, "processing_steps/pre_processed.bmp");
}

void LineDetection(SDL_Surface *image_to_process, double sobel_threshold, double hough_threshold, char *line_color)
{
	SDL_Surface *edge_image = SDL_CreateRGBSurface(0, image_to_process -> w, image_to_process -> h, 32, 0, 0, 0, 0);
	SobelEdgeDetection(image_to_process, edge_image, sobel_threshold);
	HoughTransform(edge_image, image_to_process, 180, 180, hough_threshold, line_color);
	
	Flip(image_to_process);
	Flip(edge_image);
	
	HoughTransform(edge_image, image_to_process, 180, 180, hough_threshold - 10, line_color);
	Flip(image_to_process);
	EraseImageBorders(image_to_process, 0.03 * image_to_process -> w);
	
	free(edge_image);
}

void DrawGrid(SDL_Surface *image_surface, SDL_Surface *img_copy, int px, int py)
{
	Uint32 pixel;

	pixel = SDL_MapRGB(img_copy -> format, 255, 0, 0);


	for (int i = px; i < image_surface -> h + px; i++)
	{
		if (i + 1 < img_copy -> h)
			put_pixel(img_copy, py, i + 1, pixel);
		put_pixel(img_copy, py, i, pixel);
		
		if (i - 1 > 0)
			put_pixel(img_copy, py, i - 1, pixel);
	}

	for (int i = py; i < image_surface -> w + py; i++)
	{
		if (i - 1 > 0)
			put_pixel(img_copy, i - 1, px, pixel);
		
		if (i + 1 < img_copy -> w)
			put_pixel(img_copy, i + 1, px, pixel);
		put_pixel(img_copy, i, px, pixel);
	}

	for (int i = py; i < image_surface -> w + py; i++)
	{
		if (i - 1 > 0)
			put_pixel(img_copy, i - 1, px + image_surface -> h, pixel);
		put_pixel(img_copy, i, px + image_surface -> h, pixel);
		
		if (i + 1 < img_copy -> w)
			put_pixel(img_copy, i + 1, px + image_surface -> h, pixel);
	}

	for (int i = px; i < image_surface -> h + px; i++)
	{
		if (i + 1 < img_copy -> h)
			put_pixel(img_copy, py + image_surface -> w, i + 1,pixel);
		put_pixel(img_copy, py + image_surface -> w, i,pixel);
		
		if (i - 1 > 0)
			put_pixel(img_copy, py + image_surface -> w, i - 1,pixel);
	}
	

}

void Draw(SDL_Surface *img, SDL_Surface *number, int px, int py)
{
	Uint32 pixel;
	Uint8 r, g, b;

	for (int i = 0; i < number -> h; i++)
	{
		for (int j = 0; j < number -> w; j++)
		{
			pixel = get_pixel(number, j, i);
			SDL_GetRGB(pixel, number -> format, &r, &g, &b);
			if (r == 0 && g == 0 && b == 0)
			{
				if ((px + i < img -> h) && (py + j < img -> w))
				{
					pixel = SDL_MapRGB(img -> format, 255, 0, 0);
					put_pixel(img, py + j, px + i, pixel);
				}
			}
		}
	}

}

void DrawImage(SDL_Surface *img, char *img_path, int px, int py, int grid_h, int grid_w)
{
	SDL_Surface *nb = load_image(img_path);
	
	SDL_Surface *scaled_nb = SDL_CreateRGBSurface(0, grid_w / 9, grid_h / 9, 32, 0, 0, 0, 0);
	
	SDL_SoftStretch(nb, NULL, scaled_nb, NULL);
	
	Draw(img, scaled_nb, px, py);	

	free(nb);
	free(scaled_nb);
}

void DrawImages(SDL_Surface *img, int px, int py, int h, int w, char *grid_input_path, char *grid_output_path)
{
	char path[40] = "../image_processing/numbers_to_print/0\0";

	int c1 = 0; 
	int c2 = 0;
	
	FILE *fp1 = fopen(grid_input_path, "r");
	FILE *fp2 = fopen(grid_output_path, "r");

	if (fp1 != NULL && fp2 != NULL)
		printf("opened successfully !\n");


	for (int i = px; i < h + px - (h / 9); i += (int) (h / 9)) 
	{
		
		for (int j = py; j < w + py - (w / 9); j += (int) (w / 9))
		{
			c1 = fgetc(fp1);
			c2 = fgetc(fp2);
			
			while (c1 == '\t'|| c1 == '\n' || c1 == ' ')
			{
				c1 = fgetc(fp1);
				c2 = fgetc(fp2);
			}

			if (c1 == EOF && c2 == EOF)
				break;

			if (c1 == '.' && (c2 > 48 && c2 < 58))
			{
				path[37] = c2;
				//path[37] = 'c';

				DrawImage(img, path, i, j, h, w);
			}
		}

		while (c1 == ' ' || c1 == '\n' || c1 == '\t')
		{
			c1 = fgetc(fp1);
			c2 = fgetc(fp2);
		}

		if (c1 == EOF && c2 == EOF)
			break;
	}

	fclose(fp1);
	fclose(fp2);

}

void EraseLines(SDL_Surface *img)
{	
	EraseVerticalLines(img);
	EraseHorizontalLines(img);
}

void EraseVerticalLines(SDL_Surface *img)
{
	int height = img -> h;
	int width = img -> w;

	Uint32 pixel = SDL_MapRGB(img -> format, 255, 255, 255);

	int neigh[8] = {-3, -2, -1, 0, 1, 2, 3, 4};

	for (int j = 0; j < width; j += (width / 9))
	{
		for (int i = 0; i < height; i++)
		{
			for (int k = 0; k < 8; k++)
			{
				if (j + neigh[k] < width && j + neigh[k] > 0)
				{
					put_pixel(img, j + neigh[k], i, pixel);

				}
			}
		}
	}

}

void EraseHorizontalLines(SDL_Surface *img)
{
	int height = img -> h;
	int width = img -> w;

	Uint32 pixel = SDL_MapRGB(img -> format, 255, 255, 255);

	int neigh[8] = {-3, -2, -1, 0, 1, 2, 3, 4};

	for (int i = 0; i < height; i += (height / 9))
	{
		for (int j = 0; j < width; j++)
		{
			for (int k = 0; k < 8; k++)
			{
				if (i + neigh[k] < height && i + neigh[k] > 0)
				{
					put_pixel(img, j, i + neigh[k], pixel);

				}
			}
		}
	}

}

void SaveResultImage(SDL_Surface *processed_img, SDL_Surface *copy_img, int grid_pos_x, int grid_pos_y, char *grid_input_path, char *grid_output_path, char *image_result_path)
{
	DrawGrid(processed_img, copy_img, grid_pos_x, grid_pos_y);

	DrawImages(copy_img, grid_pos_x, grid_pos_y, processed_img -> h, processed_img -> w, grid_input_path, grid_output_path);

	SDL_SaveBMP(copy_img, image_result_path);
}

void Segmentation(SDL_Surface *image_surface, int *grid_pos_x, int *grid_pos_y, double sobel_threshold, double hough_threshold)
{
	DetectBiggestBlob(image_surface, grid_pos_x, grid_pos_y);
	printf("Grid detection successful !\n");
	LineDetection(image_surface, sobel_threshold, hough_threshold, "white");
	printf("Image is Clean !\n");
	SaveImages(image_surface, 0, 0, image_surface -> w, image_surface -> h);
	printf("Images Saved !");
}


SDL_Surface *ResizeSurface(SDL_Surface *img, int new_height, int new_width)
{
	SDL_Surface *new = SDL_CreateRGBSurface(0, new_width, new_height, 32, 0, 0, 0, 0);
	SDL_SoftStretch(img, NULL, new, NULL);

	return new;

}

void EraseImageBorders(SDL_Surface *img, double wd)
{
	Uint32 white_pixel = SDL_MapRGB(img -> format, 255, 255, 255);
	DrawLine_v3(img, 1, 1, img -> h - 1, 1, wd, white_pixel);
	DrawLine_v3(img, 1, img -> w - 1, img -> h - 1, img -> w - 1, wd, white_pixel);
	DrawLine_v3(img, 1, 1, 1, img -> w - 1, wd, white_pixel);
	DrawLine_v3(img, img -> h - 1, 1, img -> w - 1, img -> h - 1, wd, white_pixel);
}
