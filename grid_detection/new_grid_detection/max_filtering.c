#include "max_filtering.h"
#include "hough_transform.h"
#include "settings.h"

void maxFilter(double image[SIZE][180], houghList* list) {
    int k, r, theta, i, j, itr, itt;

    for (k = SIZE * 180 - 1; k >= 0; k--) {
        r = list[k].r + 128;
        theta = list[k].theta;
        if (image[r][theta] == 255) {
            for (j = -4; j <= 4; j++) {
                for (i = -4; i <= 4; i++) {
                    itt = theta + i;
                    itr = r + j;
                    if (0 <= itt && itt < 180 && 0 <= itr && itr < SIZE) {
                        if (!(i == 0 && j == 0)) {
                            image[itr][itt] = 0.0;
                        }
                    }
                }
            }
        }
    }
}