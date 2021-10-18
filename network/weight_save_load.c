#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "weight_save_load.h"

void save_weights(double *weights, size_t height, size_t width,char *c)
{
    //FILE *save_file = fopen("savefile.txt", "w");
    FILE *save_file = fopen(c,"w");

    if (save_file == NULL)
    {
        err(1,"File not found");
    }

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < width; x++)
        {
            fprintf(save_file,"%lf\n", weights[y * width + x]);
        }
    }

    fclose(save_file);
}

void load_weights(double *weights, size_t height, size_t width,char *c)
{
    size_t i = 0;

    //FILE *save_file = fopen("savefile.txt", "r");
    FILE *save_file = fopen(c,"r");
    char number[10];
    char *end;

    if (save_file == NULL)
    {
        err(1,"File not found");
    }

    while (fgets(number, 10, save_file) && i < height*width)
    {
        weights[i] = strtod(number, &end);
        i += 1;
    }

    fclose(save_file);
}

/*int main()
{
    double saveweights[] = {1.54, 1.99, 6.432, 5.123};
    save_weights(saveweights, 2, 2);

    double loadweights[] = {0.0, 0.0, 0.0, 0.0};
    load_weights(loadweights, 2, 2);

    for (int i = 0; i < 4; i++)
    {
        printf("%lf\n", loadweights[i]);
    }

    return 0;
}*/
