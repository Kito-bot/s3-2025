#include <stdlib.h>
#include <stdio.h>
#include "matrice_operations.h"

int * init_matrix_int( unsigned long col, unsigned long row)
{
    int *arr = malloc(sizeof(int) * col * row);
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            *(arr + i * col + j) = 1;
            //printf("%i ",0);
        }
        //printf("\n");
        
    }
    // dont forget to free(arr) when over with it.
    return arr;
}


double * init_matrix_double( unsigned long col, unsigned long row)
{
    double *arr = malloc(sizeof(int) * col * row);
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            *(arr + i * col + j) = 0.0f;
            //printf("%i ",0);
        }
        //printf("\n");
        
    }
    //free(arr);
    return arr;
}

void print_matrice(int * arr, unsigned long col,unsigned row)
{
    for (size_t i = 0; i < row; i++)
    {
        for (size_t j = 0; j < col; j++)
        {
            int val = *(arr + i * col + j);
            printf("%i ",val);
        }
        printf("\n");
    }
    
}