#include "edge_extraction.h"
#include "settings.h"

void getEdge(double original[SIZE][SIZE], double converted[SIZE][SIZE], padding p) {
    int filter_LoG[5][5] = {{0, 0, -1, 0, 0},
                            {0, -1, -2, -1, 0},
                            {-1, -2, 16, -2, -1},
                            {0, -1, -2, -1, 0},
                            {0, 0, -1, 0, 0}};
    
    int x, y, i, j, itx, ity;

    int a = (p == same) ? 0 : 2;
    int b = SIZE - a;
    double c;

    for (y = a; y < b; y++) {
        for (x = a; x < b; x++) {
            for (i = -2; i <= 2; i++) {
                for (j = -2; j <= 2; j++) {
                    itx = x + j;
                    ity = y + i;
                    converted[y][x] += (itx < 0 || SIZE <= itx || ity < 0 || SIZE <= ity) ? 0 : (original[ity][itx] * filter_LoG[i + 2][j + 2]);
                }
            }
            converted[y][x] = (c = converted[y][x]) > 255 ? 255 : (c < 0 ? 0 : c);
        }
    }
}