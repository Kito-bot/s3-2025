#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "settings.h"
#include "file_io.h"
#include "edge_extraction.h"
#include "binary.h"
#include "hough_transform.h"
#include "quick_sort.h"
#include "max_filtering.h"


int main() {
    int x, y;
    double f[SIZE][SIZE];           /* input image for image processing */
    double g[SIZE][SIZE];           /* output image for image processing */
    unsigned char f1[SIZE][SIZE];   /* input image */
    unsigned char g1[SIZE][SIZE];   /* output image */
    //FILE *fp1, *fp2;
    FILE *fp1;
    FILE *fp2;

    // name of the input image file
    char imageName[] = "beach.bmp";//sudok.raw ----> //beach.raw


    //char originalImage[50] = "original image/";
    char originalImage[50] = "";

    // load the image
    load(f1, fp1, strcat(originalImage, imageName));

    for (x = 0; x < SIZE; x++) {
        for (y = 0; y < SIZE; y++) {
            f[x][y] = (double)f1[x][y];
            g[x][y] = 0.0;
        }
    }
    
    ////// edge extraction /// START //////////////////////

    // get the edges of the image
    getEdge(f, g, valid);

    for (y = 0; y < SIZE; y++) {
        for (x = 0; x < SIZE; x++) {
            g1[y][x] = (unsigned char)g[y][x];
        }
    }

    char edgeImage[50] = "edge image/edge_";

    // save the edge image
    save(SIZE * SIZE, g1, fp2, strcat(edgeImage, imageName));

    ////// edge extraction /// END ////////////////////////

    for (y = 0; y < SIZE; y++) {
        for (x = 0; x < SIZE; x++) {
            f[y][x] = g[y][x];
            g[y][x] = 0.0;
        }
    }

    ////// binarization /// START /////////////////////////

    // binarize the image
    binarize(SIZE, f, g, 120);

    for (y = 0; y < SIZE; y++) {
        for (x = 0; x < SIZE; x++) {
            g1[y][x] = (unsigned char)g[y][x];
        }
    }

    char binaryImage[50] = "binary image/binary_";
    
    // save the binary image
    save(SIZE * SIZE, g1, fp2, strcat(binaryImage, imageName));

    ////// binarization /// END ///////////////////////////

    for (y = 0; y < SIZE; y++) {
        for (x = 0; x < SIZE; x++) {
            f[y][x] = g[y][x];
            g[y][x] = 0.0;
        }
    }

    ////// Hough transform /// START //////////////////////

    // output image 2 for image processing
    double h[SIZE][180] = {0.0};

    // output image 2
    unsigned char g2[SIZE][180];

    // list of r-theta-count
    houghList list[SIZE * 180];

    houghTransform(f, h, list);

    for (y = 0; y < SIZE; y++) {
        for (x = 0; x < 180; x++) {
            g2[y][x] = (unsigned char)h[y][x];
        }
    }

    char houghTransformImage[80] = "Hough transform image/hough_transform_";
    
    // save the binary image
    save(SIZE * 180, g2, fp2, strcat(houghTransformImage, imageName));

    ////// Hough transform /// END ////////////////////////

    quickSort(list, 0, SIZE * 180 - 1, 0);

    ////// binarization /// START /////////////////////////

    // binarize the image
    binarize(180, h, h, 120);

    // filter the binary image
    maxFilter(h, list);
    
    for (y = 0; y < SIZE; y++) {
        for (x = 0; x < 180; x++) {
            g2[y][x] = (unsigned char)h[y][x];
        }
    }

    char FilteredBinaryHoughImage[80] = "filtered binary Hough image/filtered_binary_hough_";
    
    // save the binary image
    save(SIZE * SIZE, g2, fp2, strcat(FilteredBinaryHoughImage, imageName));

    ////// binarization /// END ///////////////////////////
    
    ////// inverse Hough transform /// START //////////////

    inverseHoughTransform(h, g);

    for (y = 0; y < SIZE; y++) {
        for (x = 0; x < SIZE; x++) {
            g1[y][x] = (unsigned char)g[y][x];
        }
    }

    char inverseHoughTransformImage[100] = "inverse Hough transform image/inverse_hough_transform_";
    
    // save the binary image
    save(SIZE * SIZE, g1, fp2, strcat(inverseHoughTransformImage, imageName));

    ////// inverse Hough transform /// END ////////////////
}
