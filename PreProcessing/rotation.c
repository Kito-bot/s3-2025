#include "rotation.h"

#define PI 3.14159256

SDL_Surface* imgRotation(SDL_Surface *img, double angle)
{
    //Rotate the SDL_Surface img from an angle angle
    int w = img -> w;
    int h = img -> h;
    int mwidth = w / 2;
    int mheight = h / 2;

    double angle_compute = -angle * PI / 180.0;

    double sina = sin(angle_compute);
    double cosa = cos(angle_compute);

    SDL_Surface* rimg = SDL_CreateRGBSurface(
            SDL_SWSURFACE,
            img->w,
            img->h, 
            img->format->BitsPerPixel,
            img->format->Rmask,
            img->format->Gmask,
            img->format->Bmask,
            img->format->Amask);


    //Initialize a white SDL_Surface with the same dimention as img
    Uint32 Wpixel = SDL_MapRGB(rimg -> format, 255, 255, 255);

    SDL_FillRect(rimg, NULL, Wpixel);



    for(int x = 0; x < w; x++)
    {
        for(int y = 0; y < h; y++)
        {
            int xt = x - mwidth;
            int yt = y - mheight;

            int xs = (int)round((cosa * xt + sina * yt) + mwidth);
            int ys = (int)round(((-sina) * xt + cosa * yt) + mheight);

            if(xs >= 0 && xs < w && ys >= 0 && ys < h) 
            {
                Uint32 pixel = get_pixel(img, x, y);

                put_pixel(rimg, xs, ys, pixel);
            }

        }
    }

    return rimg;

}


