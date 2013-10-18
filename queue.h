/* Header file for the queue.c file */
#ifndef QUEUE_H_
#define QUEUE_H_

#include "job.h"

typedef struct queue Queue;

Queue * allocate();
void deallocate(Queue *queue);
int enqueue(Queue *queue, Job * job);
Job * dequeue(Queue *queue);

#endif /* QUEUE_H_ */
