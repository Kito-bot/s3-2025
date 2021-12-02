#include "hough.h"

void DrawLine(SDL_Surface *img, int x1, int y1, int x2, int y2)
{
	Uint32 pixel = SDL_MapRGB(img -> format, 255, 0, 0);

	int dx, dy, p, x, y;

	dx = x2 - x1;
	dy = y2 - y1;

	x = x1;
	y = y1;

	p = 2 * dy - dx;

	while (x < x2)
	{
		if (p >= 0)
		{
			if (x >= 0 && x < img -> h && y >= 0 && y < img -> w)
			{
				put_pixel(img, y, x, pixel);
			}
			y += 1;
			p = p + 2 * dy - 2 * dx;

			
		}
		else
		{
			if (x >= 0 && x < img -> h && y >= 0 && y < img -> w)
			{
				put_pixel(img, y, x, pixel);
			}
			p = p + 2 * dy;
			
		}

		x += 1;
	}
}

void DrawLine_v2(SDL_Surface *img, int x1, int y1, int x2, int y2)
{
	Uint32 pixel = SDL_MapRGB(img -> format, 255, 0, 0);

	int x, y, dx, dy, p;

	x = x1;
	y = y1;

	dx = x2 - x1;

	dy = y2 - y1;

	p = 2 * dy - dx;

	while (x <= x2)
	{
		if (x >= 0 && y >= 0 && x < img -> h && y < img -> w)
		{
			put_pixel(img, y, x, pixel);
		}

		x++;

		if (p < 0)
		{
			p = p + 2 * dy;
		}
		else
		{
			p = p + 2 * dy - 2 * dx;
			y++;
		}
	}
}

void DrawLine_v3(SDL_Surface *img, int x0, int y0, int x1, int y1, float wd, Uint32 pixel_color)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx - dy, e2, x2, y2;
	float ed = dx + dy == 0 ? 1 : sqrt((float) dx * dx + (float) dy * dy);


	Uint32 pixel = pixel_color;

	for (wd = (wd + 1) / 2 ; ; )
	{
		if (x0 >= 0 && y0 >= 0 && x0 < img -> h && y0 < img -> w)
		{
			put_pixel(img, y0, x0, pixel);
		}

		e2 = err;
		x2 = x0;

		if (2 * e2 >= -dx)
		{
			for (e2 += dy, y2 = y0; e2 < ed * wd && (y1 != y2 || dx > dy); e2 += dx)
			{
				if (x0 >= 0 && x0 < img -> h && (y2 + sy) >= 0 && (y2 + sy) < img -> w)
				{
					put_pixel(img, (y2 += sy), x0, pixel);
				}
			}

			if (x0 == x1)
			{
				break;
			}

			e2 = err;
			err -= dy;
			x0 += sx;
		}

		if (2 * e2 <= dy)
		{
			for (e2 = dx - e2; e2 < ed * wd && (x1 != x2 || dx < dy); e2 += dy)
			{
				if ((x2 + sx >= 0 && x2 + sx < img -> h) && (y0 >= 0 && y0 < img -> w))
				{
					put_pixel(img, y0, x2 += sx, pixel);
				}
			}

			if (y0 == y1)
			{
				break;
			}

			err += dx;
			y0 += sy;
		}


	}
}

void Flip(SDL_Surface *img)
{
	SDL_Surface *flipped = SDL_CreateRGBSurface(0, img -> h, img -> w, 32, 0, 0, 0, 0);

	Uint32 pixel;

	for (int i = 0; i < img -> h; i++)
	{
		for (int j = 0; j < img -> w; j++)
		{
			pixel = get_pixel(img, j, i);
			put_pixel(flipped, i, j, pixel);
		}
	}

	*img = *flipped;

	free(flipped);
}

void HoughTransform(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, char *line_color)
{
	if (num_rhos == 0 || num_thetas == 0)
	{
		errx(1, "Errror Hough : incorrect parameters\n");
		return ;
	}

	int d = (int) (sqrt((edge_image -> h) * (edge_image -> h) + (edge_image -> w) * (edge_image -> w)));

	double d_theta =  181 / num_thetas;
//	double d_rho = (int) (2 * d + 1) / num_rhos;

	double thetas[181];
	double sin_thetas[181];
	double cos_thetas[181];
	double rhos[2 * d + 1];

	int step = 0;
	for (int i = 0; i < 181; i++)
	{
		thetas[i] = step;

		sin_thetas[i] = sin(thetas[i] * (M_PI / 180));
		cos_thetas[i] = cos(thetas[i] * (M_PI / 180));
		step += d_theta;
	}

	step = -d;


	for (int i = 0; i < 2 * d + 1; i++)
	{
		rhos[i] = step + i;
	}

	printf("%f    %f\n", rhos[0], rhos[2 * d]);

	// init accumulator
	int accumulator[2 * d + 1][181];

	for (int i = 0; i < 2 * d + 1; i++)
	{
		for (int j = 0; j < 181; j++)
		{
			accumulator[i][j] = 0;
		}
	}


	// begin hough
	
	Uint32 pixel;
	Uint8 r, g, _b;

	int theta;
	double rho;
	
	int half_w = edge_image -> w / 2;
	int half_h = edge_image -> h / 2;

	int rho_index = 0;


	for (int y = 0; y < edge_image -> h; y++)
	{
		for (int x = 0; x < edge_image -> w; x++)
		{
			pixel = get_pixel(edge_image, x, y);
			SDL_GetRGB(pixel, src -> format, &r, &g, &_b);

			double edge_point_1 = x - half_w;
			double edge_point_2 = y - half_h;

			if (r == 255 && g == 255 && _b == 255) // white pixel => edge pixel
			{
				for (int k = 0; k < 181; k += d_theta)
				{
					rho = (edge_point_1 * cos_thetas[k]) + (edge_point_2 * sin_thetas[k]);

					theta = thetas[k];

					double min_rho_abs = DBL_MAX;

					for (int l = 0; l <  2 * d + 1; l++)
					{
						// get rho index 
						if (fabs(rho - rhos[l]) < min_rho_abs)
						{
							rho_index = l;
							min_rho_abs = fabs(rho - rhos[l]);
						}
						if (rhos[l] > rho) // reduce hough run time: rhos array in increasing order => break if current val > rho
							break;
					}

					accumulator[rho_index][theta]++;
				}
			}
		}
	}

	double a, b, x0, y0;

	double x1, y1, x2, y2;
	
	// set line color 
	Uint32 color;
	
	if (!strcmp(line_color, "red"))
	{
		color = SDL_MapRGB(src -> format, 255, 0, 0);
	}
	
	else if (!strcmp(line_color, "green"))
	{
		color = SDL_MapRGB(src -> format, 0, 255, 0);
	}
	
	else if (!strcmp(line_color, "blue"))
	{
		color = SDL_MapRGB(src -> format, 0, 0, 255);
	}
	
	else // white
	{
		color = SDL_MapRGB(src -> format, 255, 255, 255);
	}


	for (int y = 0; y < 2 * d + 1; y++)
	{
		for (int x = 0; x < 181; x++)
		{
			if (accumulator[y][x] > threshold)
			{
				rho = rhos[y];
				theta = thetas[x];

				if (abs(theta - 45) < 10 || abs(theta - 135) < 20) // skip diagonal lines
					continue;

				if (abs(theta - 90) > 10 && abs(theta) > 10) // skewed line => skip 
					continue;

				a = cos(theta * (M_PI / 180));
				b = sin(theta * (M_PI / 180));
				
				x0 = (a * rho) + half_w;
				y0 = (b * rho) + half_h;

				x1 = (int) (x0 + 1000 * (-b));
				y1 = (int) (y0 + 1000 * (a));
				x2 = (int) (x0 - 1000 * (-b));
				y2 = (int) (y0 - 1000 * (a));

				DrawLine_v3(src, y1, x1, y2, x2, 0.03 * src -> w, color);
			}
		}
	}
}

double HoughTransformAngleDetection(SDL_Surface *edge_image, SDL_Surface *src, int num_thetas, int num_rhos, int threshold, char *line_color)
{
	if (num_rhos == 0 || num_thetas == 0)
	{
		errx(1, "Errror Hough : incorrect parameters\n");
	}

	int d = (int) (sqrt((edge_image -> h) * (edge_image -> h) + (edge_image -> w) * (edge_image -> w)));

	double d_theta =  181 / num_thetas;
//	double d_rho = (int) (2 * d + 1) / num_rhos;

	double thetas[181];
	double sin_thetas[181];
	double cos_thetas[181];
	double rhos[2 * d + 1];

	int step = 0;
	for (int i = 0; i < 181; i++)
	{
		thetas[i] = step;

		sin_thetas[i] = sin(thetas[i] * (M_PI / 180));
		cos_thetas[i] = cos(thetas[i] * (M_PI / 180));
		step += d_theta;
	}

	step = -d;


	for (int i = 0; i < 2 * d + 1; i++)
	{
		rhos[i] = step + i;
	}

	printf("%f    %f\n", rhos[0], rhos[2 * d]);

	// init accumulator
	int accumulator[2 * d + 1][181];

	for (int i = 0; i < 2 * d + 1; i++)
	{
		for (int j = 0; j < 181; j++)
		{
			accumulator[i][j] = 0;
		}
	}


	// begin hough
	
	Uint32 pixel;
	Uint8 r, g, _b;

	int theta;
	double rho;
	
	int half_w = edge_image -> w / 2;
	int half_h = edge_image -> h / 2;

	int rho_index = 0;


	for (int y = 0; y < edge_image -> h; y++)
	{
		for (int x = 0; x < edge_image -> w; x++)
		{
			pixel = get_pixel(edge_image, x, y);
			SDL_GetRGB(pixel, src -> format, &r, &g, &_b);

			double edge_point_1 = x - half_w;
			double edge_point_2 = y - half_h;

			if (r == 255 && g == 255 && _b == 255) // white pixel => edge pixel
			{
				for (int k = 0; k < 181; k += d_theta)
				{
					rho = (edge_point_1 * cos_thetas[k]) + (edge_point_2 * sin_thetas[k]);

					theta = thetas[k];

					double min_rho_abs = DBL_MAX;

					for (int l = 0; l <  2 * d + 1; l++)
					{
						// get rho index 
						if (fabs(rho - rhos[l]) < min_rho_abs)
						{
							rho_index = l;
							min_rho_abs = fabs(rho - rhos[l]);
						}
						if (rhos[l] > rho) // reduce hough run time: rhos array in increasing order => break if current val > rho
							break;
					}

					accumulator[rho_index][theta]++;
				}
			}
		}
	}

	//double a, b, x0, y0;

	//double x1, y1, x2, y2;
	
	//double sum = 0;
	//int cpt = 0;
	double max_peak = 0;
	double max_peak_theta = 0;
	/*
	// set line color 
	Uint32 color;
	
	if (!strcmp(line_color, "red"))
	{
		color = SDL_MapRGB(src -> format, 255, 0, 0);
	}
	
	else if (!strcmp(line_color, "green"))
	{
		color = SDL_MapRGB(src -> format, 0, 255, 0);
	}
	
	else if (!strcmp(line_color, "blue"))
	{
		color = SDL_MapRGB(src -> format, 0, 0, 255);
	}
	
	else // white
	{
		color = SDL_MapRGB(src -> format, 255, 255, 255);
	}
	*/
	
	
	printf("%s", line_color);


	for (int y = 0; y < 2 * d + 1; y++)
	{
		for (int x = 0; x < 181; x++)
		{
			if (accumulator[y][x] > threshold)
			{
				rho = rhos[y];
				theta = thetas[x];

				if (accumulator[y][x] > max_peak)
				{
					max_peak = accumulator[y][x];
					max_peak_theta = theta;
				}
				//if (theta > 90)
				//	continue;

				//if (abs(theta - 90) > 10 && abs(theta) > 10) // skewed line => skip 
				//	continue;
				//cpt += 1;
				//sum += theta;
				/*
				a = cos(theta * (M_PI / 180));
				b = sin(theta * (M_PI / 180));
				
				x0 = (a * rho) + half_w;
				y0 = (b * rho) + half_h;

				x1 = (int) (x0 + 1000 * (-b));
				y1 = (int) (y0 + 1000 * (a));
				x2 = (int) (x0 - 1000 * (-b));
				y2 = (int) (y0 - 1000 * (a));

				DrawLine_v3(src, y1, x1, y2, x2, 0.03 * src -> w, color);
				*/
			}
		}
	}
	/*
	double angle = 0;
	if (cpt != 0)
		angle = sum / cpt;
	
	if (angle < 0)
		angle = angle + 90;
	else
		angle = angle - 90;
	
	return angle;
	*/
	printf("Angle : %f", max_peak_theta);
	
	if (max_peak_theta > 90 && max_peak_theta < 135)
		return max_peak_theta + 180 + 360;
		
	if (max_peak_theta > 135)
		return max_peak_theta + 180 + 360;
	
	return max_peak_theta - 90 + 360;
}


