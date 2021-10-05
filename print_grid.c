#include <stdio.h>
#include <stdlib.h>

void print_grid(int grid[9][9])
{
    int k = 0;
    int l = 0;
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (grid[i][j] == -1)
            {
                printf(".");
            }
            else
            {
                printf("%d", grid[i][j]);
            }

            k++;
            if (k == 3)
            {
                printf(" ");
                k = 0;
            }
        }
        printf("\n");
        l++;
        if (l == 3)
        {
            printf("\n");
            l = 0;
        }
    }
}

int load_grid(char *)

    int main()
{
    int grid[9][9] = {{3, -1, 6, 5, -1, 8, 4, -1, -1},
                      {5, 2, -1, -1, -1, -1, -1, -1, -1},
                      {-1, 8, 7, -1, -1, -1, -1, 3, 1},
                      {-1, -1, 3, -1, 1, -1, -1, 8, -1},
                      {9, -1, -1, 8, 6, 3, -1, -1, 5},
                      {-1, 5, -1, -1, 9, -1, 6, -1, -1},
                      {1, 3, -1, -1, -1, -1, 2, 5, -1},
                      {-1, -1, -1, -1, -1, -1, -1, 7, 4},
                      {-1, -1, 5, 2, -1, 6, 3, -1, -1}};
    print_grid(grid);
    return 0;
}