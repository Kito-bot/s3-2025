#include "main.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        errx(1,"kalytera: Usage : ./kalytera <path of image>");
    }

    char *path = argv[1];
    
    display(path);

    SDL_Surface *image = load_img(path);

    // Hough test (by David)
    Hough(image);
    SDL_SaveBMP(image, "hough.png");

    return EXIT_SUCCESS;
}
