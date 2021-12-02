#pragma once
#include <stdio.h>
#include "settings.h"

void load(unsigned char f1[SIZE][SIZE], FILE *fp1, char filepath[]);

void save(int sizeOfImage, unsigned char g1[SIZE][sizeOfImage / SIZE], FILE *fp2, char filepath[]);