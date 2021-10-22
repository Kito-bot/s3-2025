#include "solver_tools.h"
#include <stdio.h>
#include <stdlib.h>

void print(int arr[N][N])
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            printf("%d ",arr[i][j]);
        printf("\n");
    }
}


int isSafe(int grid[N][N], int row, int col, int num)
{
    // Check if we find the same num
    // in the similar row , we return 0
    for (int x = 0; x <= 8; x++)
        if (grid[row][x] == num)
            return 0;
    // Check if we find the same num in the
    // similar column , we return 0
    for (int x = 0; x <= 8; x++)
        if (grid[x][col] == num)
            return 0;
    // Check if we find the same num in the
    // particular 3*3 matrix, we return 0
    int startRow = row - row % 3,
        startCol = col - col % 3;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (grid[i + startRow][j + startCol] == num)
                return 0;
    return 1;
}

void read_file(char *name, int grid[9][9])
{
    FILE *file = fopen(name, "r");
    int c;
    int i = 0;
    int j = 0;
    while ((c = fgetc(file)) != EOF)
    {
        if (c == ' ' || c == '\n' || c == '\r' || c == '\0' || c == '\f')
        {
            continue;
        }
        if (c == '.')
        {
            grid[i][j] = 0;
            j += 1;
        }
        else
        {
            grid[i][j] = (int) c - 48;
            j += 1;
        }
        if (j == 9)
        {
            i += 1;
            j = 0;
        }
    }
    fclose(file);
}

size_t my_str_len(char str[])
{
    size_t len = 0;
    while (str[len] != '\0')
    {
        len += 1;
    }
    return len;
}

void write_file(char *name, int grid[9][9])
{
    // crée le nom du fichier sous le bon format
    char result_name[my_str_len(name) + 7 + 1];
    for (size_t i = 0; i < my_str_len(name); i++)
    {
        result_name[i] = name[i];
    }

    char extension[] = ".result";
    for (size_t i = 0; i < my_str_len(extension); i++)
    {
        result_name[my_str_len(name) + i] = extension[i];
    }
    result_name[my_str_len(name) + 7] = '\0'; // dernier character 0 : fin de la string
    // fin écriture nom fichier
    FILE *file;
    file = fopen(result_name, "w");
    for (size_t i = 0; i < 9; i++)
    {
        for (size_t j = 0; j < 9; j++)
        {
            fputc(48 + grid[i][j], file);
            if ((j + 1) % 3 == 0 && (j != 8))
            {
                fputc(32,file);
            }
        }
        if ((i + 1) % 3 == 0)
        {
            fputc(10, file);
        }
        fputc(10, file);
    }
    fclose(file);
}


int solveSudoku(int grid[N][N], int row, int col)
{
    // Check if we have reached the 8th row
    // and 9th column (0
    // indexed matrix) , we are
    // returning true to avoid
    // further backtracking
    if (row == N - 1 && col == N)
        return 1;
    // Check if column value becomes 9 ,
    // we move to next row and
    // column start from 0
    if (col == N)
    {
        row++;
        col = 0;
    }
    // Check if the current position
    // of the grid already contains
    // value >0, we iterate for next column
    if (grid[row][col] > 0)
        return solveSudoku(grid, row, col + 1);
    for (int num = 1; num <= N; num++)
    {
        // Check if it is safe to place
        // the num (1-9) in the
        // given row ,col ->we move to next column
        if (isSafe(grid, row, col, num)==1)
        {
            /* assigning the num in the
               current (row,col)
               position of the grid
               and assuming our assined num
               in the position
               is correct */
            grid[row][col] = num;
            // Checking for next possibility with next
            // column
            if (solveSudoku(grid, row, col + 1)==1)
                return 1;
        }
        // Removing the assigned num ,
        // since our assumption
        // was wrong , and we go for next
        // assumption with
        // diff num value
        grid[row][col] = 0;
    }
    return 0;
}

void SolveGrid(char *grid_input) //it does all in one
{
    int grid[9][9];
    read_file(grid_input, grid);
    if (solveSudoku(grid, 0, 0) == 1)
    {
        for (size_t i = 0; i < 9; i++)
        {
            for (size_t j = 0; j < 9; j++)
            {
                printf("%i", grid[i][j]);

                if ((j + 1) % 3 == 0)
                {
                    printf(" ");
                }
            }

            printf("\n");

            if ((i + 1) % 3 == 0)
            {
                printf("\n");
            }
        }

    }
    write_file(grid_input, grid);
}
