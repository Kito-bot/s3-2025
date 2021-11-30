#include <err.h>
#include <stdio.h>
//#include<conio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "tools.h"
#include "image_split.h"

void change_pixel(SDL_Surface *image_surface, int i, int j, Uint32 pixel)
{
    Uint8 r, g, b;
    SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
    r = 255;
    g = 0;
    b = 0;
    Uint32 pixel_ = SDL_MapRGB(image_surface->format, r, g, b);

    put_pixel(image_surface, i, j, pixel_);
}

void hough_detection(SDL_Surface *image_surface, int width, int height){
    int x = 0;
    int y = 0;

    Uint32 black = SDL_MapRGB(image_surface->format, 0, 0, 0);
    //Uint32 pixel = get_pixel(image_surface, x, y);

    //int width = image_surface->w;
    //int height = image_surface->h;

    for (; x < width; x++)
    {
        Uint32 pixel = get_pixel(image_surface, x, y);
        if (pixel == black)
        {
            for (; y < height; y++)
            {    
                change_pixel(image_surface, x, y, pixel);
            }
        }
        
    }

    x = 0;
    y = 0;

    for (; y < height; y++)
    {
        Uint32 pixel = get_pixel(image_surface, x, y);
        if (pixel == black)
        {
            for (; x < width; x++)
            {    
                change_pixel(image_surface, x, y, pixel);
            }
        }
        
    }

}

void flood_filling_(SDL_Surface *image_surface, int i, int j, Uint8 old_color, Uint8 new_color)
{
    int width = image_surface->w;
    int height = image_surface->h;
    Uint32 pixel = get_pixel(image_surface, i, j);

    if (i < 0 || i >= height || j < 0 || j >= width)
        return;

    // SDL_GetRGB(pixel, image_surface->format, &r, &g, &b);
    // if (r == 0 && g == 0 && b == 0)
    //     return;
    if (pixel != old_color)
        return;
    if (pixel == new_color)
        return;

    else
    {
        Uint32 pixel_ = SDL_MapRGB(image_surface->format, 255, 0, 0);
        put_pixel(image_surface, i, j, pixel_);
        flood_filling_(image_surface, i + 1, j, old_color, new_color);
        flood_filling_(image_surface, i - 1, j, old_color, new_color);
        flood_filling_(image_surface, i, j + 1, old_color, new_color);
        flood_filling_(image_surface, i, j - 1, old_color, new_color);
    }
}

void flood_filling(SDL_Surface *image_surface, int i, int j, Uint8 new_color)
{
    Uint32 old_color = get_pixel(image_surface, i, j);

    if (old_color == new_color)
    {
        return;
    }
    flood_filling_(image_surface, i, j, old_color, new_color);
}

int main()
{
    SDL_Surface *image_surface;
    SDL_Surface *screen_surface;

    init_sdl();

    image_surface = load_image("sudoku-grid_1.png");
    screen_surface = display_image(image_surface);
    //Uint32 red_pixel = SDL_MapRGB(image_surface->format, 0, 0, 0);
    int width = image_surface->w;
    int height = image_surface->h;

    wait_for_keypressed();


    hough_detection(image_surface,width,height);

    split_image();

    //flood_filling(image_surface, 0, 0, red_pixel);
    /*for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            if (pixel == red_pixel)
            {
                change_pixel(image_surface, i, j, pixel);
            }
        }
    }*/

    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}
