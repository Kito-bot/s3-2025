#include "hough_transform.h"
#include <math.h>
#include <stdio.h>
#include "settings.h"

void houghTransform(double original[SIZE][SIZE], double converted[SIZE][180], houghList* list) {
    int x, y, r, theta;
    double radian;

    for (theta = 0; theta < 180; theta++) {
        for (y = 0; y < SIZE; y++) {
            for (x = 0; x < SIZE; x++) {
                if (original[y][x] == 255) {
                    radian = theta * PI / 180;
                    r = (int)((x - 128) * cos(radian) + (y - 128) * sin(radian));
                    if (!(-128 <= r && r <= 127)) continue;
                    if (converted[r + 128][theta] < 255) converted[r + 128][theta]++;
                }
            }
        }
    }

    for (r = 0, x = 0; r < SIZE; r++) {
        for (theta = 0; theta < 180; theta++, x++) {
            list[x].r = r - 128;
            list[x].theta = (unsigned char)theta;
            list[x].count = (unsigned char)converted[r][theta];
        }
    }
}

void inverseHoughTransform(double original[SIZE][180], double converted[SIZE][SIZE]) {
    int x, y, r, theta;
    double radian;

    for (x = 0; x < SIZE; x++) {
        for (theta = 1; theta < 180; theta++) {
            for (r = 0; r < SIZE; r++) {
                if (original[r][theta] == 255) {
                    radian = theta * PI / 180;
                    y = (int)(-(1 / tan(radian)) * (x - 128) + ((r - 128) / sin(radian)));
                    if (!(-128 <= y && y <= 127)) continue;
                    if (converted[y + 128][x] < 255) converted[y + 128][x] = 255;
                }
            }
        }
    }

    for (y = 0; y < SIZE; y++) {
        for (theta = 0; theta < 180 && theta != 90; theta++) {
            for (r = 0; r < SIZE; r++) {
                if (original[r][theta] == 255) {
                    radian = theta * PI / 180;
                    x = (int)(-tan(radian) * (y - 128) + ((r - 128) / cos(radian)));
                    if (!(-128 <= x && x <= 127)) continue;
                    if (converted[y][x + 128] < 255) converted[y][x + 128] = 255;
                }
            }
        }
    }
}