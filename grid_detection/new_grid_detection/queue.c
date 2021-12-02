#include "queue.h"

Elt *InitElt()
{
	Elt *elt = malloc(sizeof(Elt));

	elt -> point = NULL;
	elt -> next = NULL;

	return elt;
}

void Enqueue(Queue *queue, Point *point)
{
	Elt *elt = InitElt();

	elt -> point = point;

	if ((queue -> first) != NULL)
	{
		Elt *cur = queue -> first;

		while (cur -> next != NULL)
			cur = cur -> next;

		cur -> next = elt;
	}

	else
	{
		queue -> first = elt;

	}

}

int isempty(Queue *queue)
{
	if (queue -> first == NULL)
		return 1;

	return 0;
}

Elt *Dequeue(Queue *queue)
{
	Elt *elt = malloc(sizeof(Elt));

	if (isempty(queue) == 0)
	{
		if (queue -> first -> next != NULL)
		{
			elt = queue -> first;
			queue -> first = queue -> first -> next;
		}
		else
		{
			elt = queue -> first;
			queue -> first = NULL;
		}

		return elt;

	}

	return NULL;
}


void Test()
{
	Queue *queue = malloc(sizeof(Queue));
    // ============= me testin anythong ========
	/*Elt *loic = malloc(sizeof(Elt));
	loic->point = NULL;
	loic->next = NULL;
	queue -> first = loic;*/
	// =========================================
	Point* point = malloc(sizeof(Point));

	point -> x = 0;
	point -> y = 0;

	point -> visited = 1;


	Point* point2 = malloc(sizeof(Point));

	point2 -> x = 1;
	point2 -> y = 1;

	point2 -> visited = 0;

	Enqueue(queue, point);

	Enqueue(queue, point2);

	Elt *elt = Dequeue(queue);

	printf("Point : x = %i  y = %i  visited %i   ", elt -> point -> x, elt -> point -> y, elt -> point -> visited);

	Elt *elt2 = Dequeue(queue);

	printf("\nPoint : x = %i  y = %i  visited %i   ", elt2 -> point -> x, elt2 -> point -> y, elt2 -> point -> visited);

	Elt *elt3 = Dequeue(queue);

	if (elt3 == NULL)
	{
		printf("\nqueue is empty");
	}

	Point *p  = malloc(sizeof(Point));
	p -> x = 4;
	p -> y  =7;

	p-> visited = 1;

	Enqueue(queue, p);

	printf("\n%i\n", isempty(queue));

	free(queue);
	free(point);
	free(point2);
	free(elt);
	free(elt2);
	free(elt3);

}
