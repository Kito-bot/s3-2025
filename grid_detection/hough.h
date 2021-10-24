#include "tools.h"

#ifndef M_PI
#define M_PI 3.1415927
#endif

#define GR(X, Y) (d[(*s) * (Y) + bpp * (X) + ((2) % bpp)])
#define GG(X, Y) (d[(*s) * (Y) + bpp * (X) + ((1) % bpp)])
#define GB(X, Y) (d[(*s) * (Y) + bpp * (X) + ((0) % bpp)])
#define SR(X, Y) (ht[4 * tw * ((Y) % th) + 4 * ((X) % tw) + 2])
#define SG(X, Y) (ht[4 * tw * ((Y) % th) + 4 * ((X) % tw) + 1])
#define SB(X, Y) (ht[4 * tw * ((Y) % th) + 4 * ((X) % tw) + 0])
#define RAD(A) (M_PI * ((double)(A)) / 180.0)

double to_radian(int d);
int houghTransform(SDL_Surface *image, int **acc);