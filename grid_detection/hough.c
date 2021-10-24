#include "hough.h"

double to_radian(int d)
{
    return d * (M_PI / 180.0);
}

int houghTransform(SDL_Surface *image, int **acc)
{
    int w = image->w;
    int h = image->h;
    int r_max = (int)sqrt(w * w + h * h);
    double rho, rad;
    Uint32 pixel;
    Uint8 r, g, b;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {
            pixel = get_pixel(image, x, y);
            SDL_GetRGB(pixel, image->format, &r, &g, &b);

            if (r == 255)
            {
                for (int theta = 0; theta < 100; ++theta)
                {
                    rad = to_radian(theta);
                    rho = x * cos(rad) + y * sin(rad);
                    acc[(int)rho + r_max][theta] += 1;
                }
            }
        }
    }
    return r_max;
}
