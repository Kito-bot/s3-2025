#include <err.h>
#include <stdio.h>
//#include<conio.h>
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "tools.h"
#include "pixel_operations.h"

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
    SDL_Surface *image_surface = load_image("sudoku-grid.png");
    SDL_Surface *screen_surface = display_image(image_surface);
    init_sdl();
    Uint32 red_pixel = SDL_MapRGB(image_surface->format, 255, 0, 0);
    //old_color = pixel;

    flood_filling(image_surface, 0, 0, red_pixel);
    //display_image(image_surface);

    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}
