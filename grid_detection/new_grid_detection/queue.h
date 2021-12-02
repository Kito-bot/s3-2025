#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <SDL.h>

// queue structure to perform iterative flood fill

// Point: to represent a pixel

typedef struct Point Point;
struct Point
{
	int x;
	int y;

	int visited;
};

typedef struct Elt Elt;
struct Elt
{
	Point *point;

	Elt *next;
};

typedef struct Queue Queue;
struct Queue
{
	Elt *first;
};

/*---------------------------------------------------------------*/


Elt *InitElt();

int isempty(Queue *queue);

void Enqueue(Queue *queue, Point *point);

Elt *Dequeue(Queue *queue);

void Test();
#endif
