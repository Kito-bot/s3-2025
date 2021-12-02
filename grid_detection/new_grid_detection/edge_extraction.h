#pragma once
#include "settings.h"

typedef enum PADDING {same, valid} padding;

void getEdge(double original[SIZE][SIZE], double converted[SIZE][SIZE], padding p);