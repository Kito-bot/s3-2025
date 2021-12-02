#pragma once
#include "settings.h"

typedef struct houghList {
    char r;
    unsigned char theta;
    unsigned char count;
} houghList;

void houghTransform(double original[SIZE][SIZE], double converted[SIZE][180], houghList* list);

void inverseHoughTransform(double original[SIZE][180], double converted[SIZE][SIZE]);