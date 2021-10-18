#include <err.h>
#include <stdio.h>
//#include<conio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "tools.h"

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

void flood_filling_(SDL_Surface *image_surface, int i, int j, Uint8 old_color, Uint8 new_color)
{
    int width = image_surface->w;
    int height = image_surface->h;
    Uint32 pixel = get_pixel(image_surface, i, j);

    if (i < 0 || i >= height || j < 0 || j >= width)
        return;
    if (pixel != old_color)
        return;
    if (pixel == new_color)
        return;

    else
    {
        change_pixel(image_surface, i, j, pixel);
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

    image_surface = load_image("sudoku_preprocessed.png");
    screen_surface = display_image(image_surface);
    Uint32 red_pixel = SDL_MapRGB(image_surface->format, 255, 255, 255);
    int width = image_surface->w;
    int height = image_surface->h;

    wait_for_keypressed();

    //flood_filling(image_surface, 0, 0, red_pixel);
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height; j++)
        {
            Uint32 pixel = get_pixel(image_surface, i, j);
            if (pixel == red_pixel)
            {
                change_pixel(image_surface, i, j, pixel);
            }
        }
    }

    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}
