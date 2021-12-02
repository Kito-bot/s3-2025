#include "file_io.h"
#include <stdio.h>
#include <stdlib.h>
#include "settings.h"

void load(unsigned char f1[SIZE][SIZE], FILE *fp1, char filepath[]) {
    if ((fp1 = fopen(filepath, "rb")) == NULL) {
        printf("Failed to load file.");
        exit(EXIT_FAILURE);
    };
    fread(f1, 1, SIZE * SIZE, fp1);
    fclose(fp1);
}

void save(int sizeOfImage, unsigned char g1[SIZE][sizeOfImage / SIZE], FILE *fp2, char filepath[]) {
    if ((fp2 = fopen(filepath, "wb")) == NULL) {
        printf("Failed to save file.");
        exit(EXIT_FAILURE);
    }
    fwrite(g1, sizeof(unsigned char), sizeOfImage, fp2);
    fclose(fp2);
}