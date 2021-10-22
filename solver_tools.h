#ifndef SOLVER_TOOLS_H_
#define SOLVER_TOOLS_H_

#include <stdio.h>
#include <stdlib.h>

// N is the size of the 2D matrix N*N
#define N 9

/* A utility function to print grid */
void print(int arr[N][N]);

// Checks whether it will be legal
// to assign num to the
// given row, col
int isSafe(int grid[N][N], int row, int col, int num);

/* Takes a partially filled-in grid and attempts
to assign values to all unassigned locations in
such a way to meet the requirements for
Sudoku solution (non-duplication across rows,
columns, and boxes) */
int solveSudoku(int grid[N][N], int row, int col);


#endif
