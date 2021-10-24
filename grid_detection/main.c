#include "hough.h"
#include "tools.h"

int main()
{
    SDL_Surface *image_surface = load_image("test_sudoku.jpeg");
    SDL_Surface *screen_surface = display_image(image_surface);
    init_sdl();

    houghTransform(image_surface, 0);
    //display_image(image_surface);

    update_surface(screen_surface, image_surface);
    wait_for_keypressed();

    SDL_FreeSurface(image_surface);
    SDL_FreeSurface(screen_surface);
    return 0;
}