#include <err.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <sys/stat.h>
#include "operations.h"
#include <math.h>

//#include "image_processing.h"

#ifndef M_PI
#define M_PI 3.1415926535897932
#endif
//#include <SDL_gfx.h>


void init_sdl()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1)
	errx(1,"Could not initialize SDL: %s .\n", SDL_GetError());
}

SDL_Surface* load_image(char *path)
{
    SDL_Surface *img;
    
    img = IMG_Load(path);
    if (!img)
        errx(3, "can't load %s: %s", path, IMG_GetError());
        
    return img;
}


Uint8* pixel_ref(SDL_Surface *surf, unsigned x, unsigned y)
{
    int bpp = surf->format->BytesPerPixel;
    return (Uint8*)surf->pixels + y * surf->pitch + x * bpp;
}


Uint32 get_pixel(SDL_Surface *surface, unsigned x, unsigned y)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch (surface->format->BytesPerPixel)
    {
        case 1:
            return *p;

        case 2:
            return *(Uint16 *)p;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;

        case 4:
            return *(Uint32 *)p;
    }

    return 0;
}

void put_pixel(SDL_Surface *surface, unsigned x, unsigned y, Uint32 pixel)
{
    Uint8 *p = pixel_ref(surface, x, y);

    switch(surface->format->BytesPerPixel)
    {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *)p = pixel;
            break;

        case 3:
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            }
            else
            {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *)p = pixel;
            break;
    }
}

void ApplyGreyscale(SDL_Surface *surface)
{
	int width = surface->w;
    	int height = surface->h;
    	
    	for (int x = 0; x < width; x++)
    	{
		for (int y = 0; y < height; y++)
		{
			Uint32 pixel = get_pixel(surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, surface->format, &r, &g, &b);
			Uint32 average = 0.3 * r + 0.59 * g + 0.11 * b;
			r = g = b = average;
			pixel = SDL_MapRGB(surface->format, r, g, b);
			put_pixel(surface, x, y, pixel);
		}
	}
}

void ApplyBlackAndWhite(SDL_Surface *surface, Uint32 threshold)
{
	int width = surface->w;
    	int height = surface->h;
    	
    	for (int x = 0; x < width; x++)
    	{
		for (int y = 0; y < height; y++)
		{
			Uint32 pixel = get_pixel(surface, x, y);
			Uint8 r, g, b;
			SDL_GetRGB(pixel, surface->format, &r, &g, &b);
			Uint32 average = (r + g + b) / 3;
			
			if (average < threshold)
				average = 0;
			else
				average = 255;
			
			r = g = b = average;
			pixel = SDL_MapRGB(surface->format, r, g, b);
			put_pixel(surface, x, y, pixel);
		}
	}
}

SDL_Surface* RotateSurface(SDL_Surface *surface, double angleInDegrees)
{
	double angle = angleInDegrees * (M_PI / 180.0);
	double cosine = cos(angle);
	double sine = sin(angle);
	
	int height = surface->h;
	int width = surface->w;
	
	int new_height = round(fabs(height * cosine) + fabs(width * sine)) + 1;
	int new_width = round(fabs(width * cosine) + fabs(height * sine)) + 1;
	
	SDL_Surface* rotated_surface = SDL_CreateRGBSurface(0, new_width, new_height, 32, 0, 0, 0, 0);
	
	int original_center_height = round(((height + 1) / 2) - 1);
	int original_center_width = round(((width + 1) / 2) - 1);
	
	int new_center_height = round(((new_height + 1) / 2) - 1);
 	int new_center_width = round(((new_width + 1) / 2) - 1);
 	
 	for (int i = 0; i < height; i++)
 	{
 		for (int j = 0; j < width; j++)
    		{
    			int y = height - 1 - i - original_center_height;
    			int x = width - 1 - j - original_center_width;
    			
    			int new_y = round(-x * sine + y * cosine);
    			int new_x = round(x * cosine + y * sine);
    			
    			new_y = new_center_height - new_y;
    			new_x = new_center_width - new_x;
            			
            		if ((0 <= new_x && new_x < new_width) && (0 <= new_y && new_y < new_height) && new_x >= 0 && new_y >= 0)
            		{
            			Uint32 pixel = get_pixel(surface, j, i);
            			put_pixel(rotated_surface, new_x, new_y, pixel);
            		}
    		}
    	}
    	
    	return rotated_surface;
}


void Shear(double angle, int* xPtr, int* yPtr)
{
	// shear 1
	double tangent = tan(angle / 2);
	int x = *xPtr;
	int y = *yPtr;
	int new_x = round(x - y * tangent);
	int new_y = *yPtr;
	
	//shear 2
	new_y = round(new_x * sin(angle) + new_y);
	
	//shear 3
	new_x = round(new_x - new_y * tangent);
	
	*xPtr = new_x;
	*yPtr = new_y;
}

SDL_Surface* ShearSurface(SDL_Surface *surface, int angleInDegrees)
{
	double angle = -(angleInDegrees * (M_PI / 180.0));
	double cosine = cos(angle);
	double sine = sin(angle);
	
	int height = surface->h;
	int width = surface->w;
	
	int new_height = round(fabs(height * cosine) + fabs(width * sine)) + 1;
	int new_width = round(fabs(width * cosine) + fabs(height * sine)) + 1;
	
	SDL_Surface* rotated_surface = SDL_CreateRGBSurface(0, new_width, new_height, 32, 0, 0, 0, 0);
	
	int original_center_height = round(((height + 1) / 2) - 1);
	int original_center_width = round(((width + 1) / 2) - 1);
	
	int new_center_height = round(((new_height + 1) / 2) - 1);
 	int new_center_width = round(((new_width + 1) / 2) - 1);
 	
 	for (int i = 0; i < height; i++)
 	{
 		for (int j = 0; j < width; j++)
    		{
    			int y = height - 1 - i - original_center_height;
    			int x = width - 1 - j - original_center_width;
    			
    			//int new_y = round(-x * sine + y * cosine);
    			//int new_x = round(x * cosine + y * sine);
    			
    			int *yPtr = &y;
    			int* xPtr = &x;
    			int new_y = *yPtr;
    			Shear(angle, xPtr, yPtr);
    			int new_x = *xPtr;
    			
    			new_y = new_center_height - new_y;
    			new_x = new_center_width - new_x;
            		
            		Uint32 pixel = get_pixel(surface, j, i);
            		put_pixel(rotated_surface, new_x, new_y, pixel);
    		}
    	}
    	
    	return rotated_surface;
}

void loop1 (SDL_Surface *surface, int *yt, int *middlet, int *failt, int *possiblet, int *i2t)
{
	int width = surface->w;
	int height = surface->h;
	int middle = *middlet;
	int y = *yt;
	int possible = *possiblet;
	int fail = *failt;
	int i2 = *i2t;
	while (middle + i2<width-1 && possible == 0 && y>3 && y<height-3)     //if true then y=y+... end =1. Next line if end ==0...
	{		
	  	Uint32 pixel = get_pixel(surface, middle+i2+fail, y);
	     	Uint8 r3, g3, b3;
		SDL_GetRGB(pixel, surface->format, &r3, &g3, &b3);
	      	if(r3!=0 && r3!=220)
		{
			Uint32 pixel = get_pixel(surface, middle+i2+fail, y+1);
			Uint8 r4, g4, b4;
			SDL_GetRGB(pixel, surface->format, &r4, &g4, &b4);
			if(r4!=0 && r4!=220)
			{
		 		Uint32 pixel = get_pixel(surface, middle+i2+fail, y-1);
			    	Uint8 r5, g5, b5;
			    	SDL_GetRGB(pixel, surface->format, &r5, &g5, &b5);
			     	if(r5!=0 && r5 != 220)
				{
					Uint32 pixel = get_pixel(surface, middle+i2+fail, y+2);
					Uint8 r6, g6, b6;
					SDL_GetRGB(pixel, surface->format, &r6, &g6, &b6);
					if(r6!=0 && r6 != 220)
					{
						Uint32 pixel = get_pixel(surface, middle+i2+fail, y-2);
						Uint8 r7, g7, b7;
						SDL_GetRGB(pixel, surface->format, &r7, &g7, &b7);
						if(r7!=0 && r7 != 220)
						{
							if (fail == 4)
							{
								possible = 1;
							}
							fail = fail +1;
						}
						else
						{
							y=y-2;
							fail =0;
						}
					}
					else
					{
						y=y+2;
						fail=0;
					}
				}
				else
				{
					y=y-1;
					fail=0;
				}
			}
			else
			{
				y=y+1;
				fail=0;
			}
		}
		else
		{
			i2=i2+1;
			fail=0;
		}
		int r1 = 220;
		pixel = SDL_MapRGB(surface->format, r1, g3, b3);
		put_pixel(surface, middle+i2+fail, y, pixel);
		i2=i2+1;
		*middlet= middle;
		*i2t = i2;
		*failt = fail;
		*possiblet = possible;
		*yt = y;	
	}
}

void loop2 (SDL_Surface *surface, int *yt, int *middlet, int *failt, int *possiblet, int *i2t)
{
	int width = surface->w;
	int height = surface->h;
	int middle = *middlet;
	int y = *yt;
	int possible = *possiblet;
	int fail = *failt;
	int i2 = *i2t;
	while(middle>width-5)
	{
		middle=middle-1;
	}
	while (i2<height-3-y && possible == 0 && middle>3 && middle <width-3)
	{
		Uint32 pixel = get_pixel(surface, middle, y+fail+i2);
     		Uint8 r3, g3, b3;
		SDL_GetRGB(pixel, surface->format, &r3, &g3, &b3);
	      	if(r3!=0 && r3!=220)
		{
			Uint32 pixel = get_pixel(surface, middle-1, y+i2+fail);
			Uint8 r4, g4, b4;
			SDL_GetRGB(pixel, surface->format, &r4, &g4, &b4);
			if(r4!=0 && r4!=220)
			{
		 		Uint32 pixel = get_pixel(surface, middle+1, y+i2+fail);
			    	Uint8 r5, g5, b5;
			    	SDL_GetRGB(pixel, surface->format, &r5, &g5, &b5);
			     	if(r5!=0 && r5 != 220)
				{
					Uint32 pixel = get_pixel(surface, middle-2, y+i2+fail);
					Uint8 r6, g6, b6;
					SDL_GetRGB(pixel, surface->format, &r6, &g6, &b6);
					if(r6!=0 && r6 != 220)
					{
						Uint32 pixel = get_pixel(surface, middle+2, y+i2+fail);
						Uint8 r7, g7, b7;
						SDL_GetRGB(pixel, surface->format, &r7, &g7, &b7);
						if(r7!=0 && r7 != 220)
						{
							if (fail == 4)
							{
								possible = 1;
							}
							fail = fail +1;
						}
						else
						{
							middle=middle+2;
							fail =0;
						}
					}
					else
					{
						middle=middle-2;
						fail=0;
					}
				
					
				}
				else
				{
					middle=middle+1;
					fail=0;
				}
			}
			else
			{
				middle=middle-1;
				fail=0;
			}
		}
		else
		{
			i2=i2+1;
			fail=0;
		}
		int r1 = 220;
		pixel = SDL_MapRGB(surface->format, r1, g3, b3);
		put_pixel(surface, middle, y+i2+fail, pixel);
		i2=i2+1;
		*middlet= middle;
		*i2t = i2;
		*failt = fail;
		*possiblet = possible;
		*yt = y;	
	}
}
void loop3 (SDL_Surface *surface, int *yt, int *middlet, int *failt, int *possiblet, int *i2t)
{
	int height = surface->h;
	int middle = *middlet;
	int y = *yt;
	int possible = *possiblet;
	int fail = *failt;
	int i2 = *i2t;
	while(y>height-5)
	{
		y=y-1;
	}
	while (middle-i2>2 && possible == 0 && y>3 && y<height-3)
		{
	  		Uint32 pixel = get_pixel(surface, middle-fail-i2, y);
	     		Uint8 r3, g3, b3;
			SDL_GetRGB(pixel, surface->format, &r3, &g3, &b3);
		      	if(r3!=0 && r3!=220)
			{
				Uint32 pixel = get_pixel(surface, middle-fail-i2, y-1);
				Uint8 r4, g4, b4;
				SDL_GetRGB(pixel, surface->format, &r4, &g4, &b4);
				if(r4!=0 && r4!=220)
				{
			 		Uint32 pixel = get_pixel(surface, middle-fail-i2, y+1);
				    	Uint8 r5, g5, b5;
				    	SDL_GetRGB(pixel, surface->format, &r5, &g5, &b5);
				     	if(r5!=0 && r5 != 220)
					{
						Uint32 pixel = get_pixel(surface, middle-fail-i2, y-2);
						Uint8 r6, g6, b6;
						SDL_GetRGB(pixel, surface->format, &r6, &g6, &b6);
						if(r6!=0 && r6 != 220)
						{
							Uint32 pixel = get_pixel(surface, middle-fail-i2, y+2);
							Uint8 r7, g7, b7;
							SDL_GetRGB(pixel, surface->format, &r7, &g7, &b7);
							if(r7!=0 && r7 != 220)
							{
								if (fail == 4)
								{
									possible = 1;
								}
								fail = fail +1;
							}
							else
							{
								y=y+2;
								fail =0;
							}
						}
						else
						{
							y=y-2;
							fail=0;
						}
						
						
					}
					else
					{
						y=y+1;
						fail=0;
					}
				}
				else
				{
					y=y-1;
					fail=0;
				}
			}
			else
			{
				i2=i2+1;
				fail=0;
			}
			int r1 = 220;
			pixel = SDL_MapRGB(surface->format, r1, g3, b3);
			put_pixel(surface, middle-i2-fail, y, pixel);
			i2=i2+1;
		}
					
		*middlet= middle;
		*i2t = i2;
		*failt = fail;
		*possiblet = possible;
		*yt = y;	
}

void loop4 (SDL_Surface *surface, int *yt, int *middlet, int *failt, int *possiblet, int *i2t)
{
	int width = surface->w;
	int middle = *middlet;
	int y = *yt;
	int possible = *possiblet;
	int fail = *failt;
	int i2 = *i2t;
	while (middle<5)
	{
		middle=middle+1;
	}
	while (y-i2>2 && possible == 0 && middle>3 && middle <width-3)
	{
  		Uint32 pixel = get_pixel(surface, middle, y-fail-i2);
		Uint8 r3, g3, b3;
		SDL_GetRGB(pixel, surface->format, &r3, &g3, &b3);
	      	if(r3!=0 && r3!=220)
		{
			Uint32 pixel = get_pixel(surface, middle+1, y-fail-i2);
			Uint8 r4, g4, b4;
			SDL_GetRGB(pixel, surface->format, &r4, &g4, &b4);
			if(r4!=0 && r4!=220)
			{
		 		Uint32 pixel = get_pixel(surface, middle-1, y-fail-i2);
			    	Uint8 r5, g5, b5;
		    		SDL_GetRGB(pixel, surface->format, &r5, &g5, &b5);
			     	if(r5!=0 && r5 != 220)
				{
					Uint32 pixel = get_pixel(surface, middle+2, y-fail-i2);
					Uint8 r6, g6, b6;
					SDL_GetRGB(pixel, surface->format, &r6, &g6, &b6);
					if(r6!=0 && r6 != 220)
					{
						Uint32 pixel = get_pixel(surface, middle-2, y-fail-i2);
						Uint8 r7, g7, b7;
						SDL_GetRGB(pixel, surface->format, &r7, &g7, &b7);
						if(r7!=0 && r7 != 220)
						{
							if (fail == 4)
							{
								possible = 1;
							}
							fail = fail +1;
						}
						else
						{
							middle=middle-2;
							fail =0;
						}
					}
					else
					{
						middle=middle+2;
						fail=0;
					}
				}
				else
				{
					middle=middle-1;
					fail=0;
				}
			}
			else
			{
				middle=middle+1;
				fail=0;
			}
		}
		else
		{
			i2=i2+1;
			fail=0;
		}
		int r1 = 220;
		pixel = SDL_MapRGB(surface->format, r1, g3, b3);
		put_pixel(surface, middle, y-i2-fail, pixel);
		i2=i2+1;
	}
		*middlet= middle;
		*i2t = i2;
		*failt = fail;
		*possiblet = possible;
		*yt = y;	
}
void mod(SDL_Surface *surface, SDL_Surface *img, int x, int xx, int y, int yy)
{	
	int originx = x;
	int originy =y;
	
	while(y<yy)
	{
		while(x<xx)
		{
			Uint32 pixel = get_pixel(surface, x, y);
			put_pixel(img, x-originx, y-originy, pixel);
			x=x+1;
		}
		x = originx;
		y=y+1;
	}
}

void Getboxes(SDL_Surface *surface, int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, char name[])
{
	// "testFolder/Box00.bmp";
	int i = 0;
	int i2 = 0;
	SDL_Surface *img;
	int topx=0;
	int botx=0;
	int topy=0;
	int boty=0;
	while (i2 < 9)
	{
		while (i<9)
		{	
			name[12]=(char)(i+48);
			name[13]=(char)(i2+48);
			int x=(-(x4+(x3-x4)/9*i2)+(x1+(x2-x1)/9*i2))/9*i+x4+(x3-x4)/9*i2;
			int xx=(-(x4+(x3-x4)/9*i2)+(x1+(x2-x1)/9*i2))/9*(i+1)+x4+(x3-x4)/9*i2;
			int xxx=(-(x4+(x3-x4)/9*(i2+1))+(x1+(x2-x1)/9*(i2+1)))/9*(i+1)+x4+(x3-x4)/9*(i2+1);
			int xxxx=(-(x4+(x3-x4)/9*(i2+1))+(x1+(x2-x1)/9*(i2+1)))/9*i+x4+(x3-x4)/9*(i2+1);
			if (xxxx>xxx)
			{
				botx=xxxx;
			}
			else
			{
				botx=xxx;
			}
			if (x>xx)
			{
				topx=xx;
			}
			else
			{
				topx=x;
			}
			int y=(-(y4+(y1-y4)/9*i)+y3+(y2-y3)/9*i)/9*i2+y4+(y1-y4)/9*(i);
			int yy=(-(y4+(y1-y4)/9*(i+1))+y3+(y2-y3)/9*(i+1))/9*i2+y4+(y1-y4)/9*(i+1);
			int yyy=(-(y4+(y1-y4)/9*(i+1))+y3+(y2-y3)/9*(i+1))/9*(i2+1)+y4+(y1-y4)/9*(i+1);
			int yyyy=(-(y4+(y1-y4)/9*i)+y3+(y2-y3)/9*i)/9*(i2+1)+y4+(y1-y4)/9*i;
			if (y>yy)
			{
				topy=yy;
			}
			else
			{
				topy=y;
			}
			if (yyy>yyyy)
			{
				boty=yyyy;
			}
			else
			{
				boty=yyy;
			}
			boty = yyyy/2+yyy/2;
			topy = y/2 + yy/2;
			
			int para1 = botx - topx;
			int para2 = boty - topy;
			
			if(para1 < 0)
			{
				para1 = para1 * (-1);
			}
			if(para2 < 0)
			{
				para2 = para2 * (-1);
			}

			img=SDL_CreateRGBSurface(0,para1,para2,32,0,0,0,0);
			mod(surface, img, topx,botx,topy,boty);
			
			SDL_SaveBMP(img,name);
			i=i+1;
		}
		i=0;
		i2=i2+1;
	}
}



void FindCorners(SDL_Surface *surface, char name[])
{
	int width = surface->w;
	int height = surface->h;
	int middle = width/2;
	int y = 0;
	int found = 0;
	int i = 3;
	int x1 = 0;
	int y1 = 0;
	int x2 =0;
	int y2 =0;
	int x3 = 0;
	int y3 = 0;
	int x4 = 0;
	int y4 = 0;
	while (i < height && found == 0)
	{
		Uint32 pixel = get_pixel(surface, middle, i);
		Uint8 r1, g1, b1;
		SDL_GetRGB(pixel, surface->format, &r1, &g1, &b1);

		if (r1 == 0)
		{
			int i2 = 1;
			int possible = 0;
			y = i;
			int fail = 0;
			int* yt = &y;
			int* middlet = &middle;
			int* i2t = &i2;
			int* possiblet = &possible;
			int* failt = &fail;
			loop1(surface, yt, middlet, failt, possiblet, i2t);
			middle = *middlet;
			y = *yt;
			possible = *possiblet;
			fail = *failt;
			i2 = *i2t;
			x1 = middle+i2-fail;
			y1 = y;

			if (possible == 1 && i2 < width / 8)
			{
				i = i + 2;
				y = 0;
				found = 0;
			}
			else
			{
				middle = middle + i2 - fail - 2;
				i2 = 1;
				fail = 0;
				possible = 0;
				*yt= y;
				*middlet = middle;
				*i2t = i2;
				*possiblet = possible;
				*failt = fail;
				loop2(surface, yt, middlet, failt, possiblet, i2t);
				middle = *middlet;
				y = *yt;
				possible = *possiblet;
				fail = *failt;
				i2 = *i2t;
				x2 = middle;
				y2 = y + i2 - fail;

				if (possible == 1 && i2 < height / 4)
				{
					i = i + 2;
					y = 0;
					found = 0;
					middle = width / 2;
				}
				else
				{
					y = y + i2 - fail - 2;
					i2 = 1;
					fail = 0;
					possible = 0;
					*yt = y;
					*middlet = middle;
					*i2t = i2;
					*possiblet = possible;
					*failt = fail;
					loop3(surface, yt, middlet, failt, possiblet, i2t);
					middle = *middlet;
					y = *yt;
					possible = *possiblet;
					fail = *failt;
					i2 = *i2t;
					x3 = middle - i2 + fail;
					y3 = y;

					if (possible == 1 && i2 < width / 3)
					{
						i = i + 2;
						y = 0;
						found = 0;
						middle = width / 2;
					}
					else
					{
						middle = middle - i2 + fail;
						i2 = 1;
						fail = 0;
						possible = 0;
						y = y + i2 - fail - 2;
						i2 = 1;
						fail = 0;
						possible = 0;
						*yt = y;
						*middlet = middle;
						*i2t = i2;
						*possiblet = possible;
						*failt = fail;
						loop4(surface, yt, middlet, failt, possiblet, i2t);
						middle = *middlet;
						y = *yt;
						possible = *possiblet;
						fail = *failt;
						i2 = *i2t;
						x4 = middle;
						y4 = y -i2 + fail;

						if (possible == 1 && i2 < height / 3)
						{
							i = i + 2;
							y = 0;
							found = 0;
							middle = width / 2;
						}
						else
						{
							found =1;
						}
					}
				}
			}
		}
		else
		{
		  	i=i+1;
		}
	}
	//char test[] = "testFolder/Box00.bmp";
	Getboxes(surface, x1, y1,x2, y2, x3, y3, x4, y4, name);
}

void CreateFolder(char* folderName)
{
	mkdir(folderName, S_IRWXU);
}
