#include "binary.h"
#include "settings.h"

void binarize(int size_x, double original[SIZE][size_x], double converted[SIZE][size_x], unsigned char threshold) {
    int x, y;

    for (y = 0; y < SIZE; y++) {
        for (x = 0; x < size_x; x++) {
            converted[y][x] = original[y][x] >= threshold ? 255 : 0;
        }
    }
}