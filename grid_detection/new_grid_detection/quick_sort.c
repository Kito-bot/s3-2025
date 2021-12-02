#include "quick_sort.h"
#include "hough_transform.h"

// sort in ascending order
void quickSort(houghList* x, int begin, int end, int depth) {
    int i, j;
    int pivot;
    unsigned char x_pivot;
    houghList t;

    pivot = (begin + end) / 2;

    x_pivot = x[pivot].count;

    i = begin;
    j = end;

    while (1) {
        while (x_pivot > x[i].count) i++;

        while (x_pivot < x[j].count) j--;

        if (i >= j) break;

        t = x[i];
        x[i] = x[j];
        x[j] = t;

        i++;
        j--;
    }

    depth++;

    if (begin < i - 1) quickSort(x, begin, i - 1, depth);
    if (j + 1 < end) quickSort(x, j + 1, end, depth);
}