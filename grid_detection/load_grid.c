#include <stdio.h>
#include <stdlib.h>

int *load_grid(char *grid)
{
    int *final_grid = malloc(9 * sizeof(int));
    int *inter_grid = malloc(9 * sizeof(int));

    for (int i = 0; i < 9; i++)
    {
        int k = 0;
        int p = 0;
        int index = 0;
        int m = 1;
        for (int j = index; j < 18 * m; j += 2)
        {
            inter_grid[k] = grid[j];
            k++;
            printf("%d", grid[j]);
        }
        index++;
        m++;
        final_grid[p] = *inter_grid;
        p++;
        printf("\n");
    }
    free(inter_grid);
    free(final_grid);
    return final_grid;
}

int main()
{
    char *grid = "3 1 6 5 7 8 4 9 2 \n5 2 9 1 3 4 7 6 8 \n4 8 7 6 2 9 5 3 1 \n2 6 3 4 1 5 9 8 7 \n9 7 4 8 6 3 1 2 5 \n8 5 1 7 9 2 6 4 3 \n1 3 8 9 4 7 2 5 6 \n6 9 2 3 5 1 8 7 4 \n7 4 5 2 8 6 3 1 9 ";
    int *new_grid = load_grid(grid);
    return 0;
}