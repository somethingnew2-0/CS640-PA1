/* Implements a Queue structure and methods to allocate space for the queue, */
/* enqueue, and dequeue*/
#include <stdio.h>
#include <stdlib.h>
#include "job.h"
#include "queue.h"

/* A node in a linked list */
typedef struct node {
	/* a job of the node */
	Job * job;

	/* the next node in the list */
	struct node * next;
}Node;

struct queue {
	/* the size of the queue */
	int size;

	/* a pointer to the head of the queue */
	Node * head;

	/* a pointer to the tail of the queue */
	Node * tail;
};

/* Allocates memory for the queue and sets initial values */
Queue * allocate(){
	Queue * queue = (Queue *)malloc(sizeof(Queue));
	if(NULL == queue){
		printf("malloc error");
		exit(EXIT_FAILURE);
	}
	queue->size = 0;
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

/* Frees the memory allocated for this queue*/
void deallocate(Queue * queue){
	if(NULL == queue){
		return;
	}
	while(queue->size > 0){
		dequeue(queue);
	}
	free(queue);
}

/* Links the given item into the queue and returns the queue size.
 * Returns -1 if bad data is passed in.
 */
int enqueue(Queue * queue, Job * job){
	Node * node;
	if(NULL == queue || NULL == job){
		return -1;
	}
	node = (Node *)malloc(sizeof(Node));
	if(NULL == node){
		printf("malloc error");
		exit(EXIT_FAILURE);
	}

	/* link in the created node */
	node->job = job;
	node->next = NULL;
	if(queue->size == 0){
		queue->head = node;
		queue->tail = node;
	}
	else{
		(queue->tail)->next = node;
		queue->tail = node;
	}
	queue->size++;
	return queue->size;
}

/* Takes an item from the head of the queue and returns it. The pointer to*/
/* head is incremented.*/
Job * dequeue(Queue * queue){
	Node * temp;
	Job * job;
	if(NULL == queue || 0 == queue->size){
		return NULL;
	}
	temp = queue->head;
	job = temp->job;
	queue->head = temp->next;
	free(temp);
	queue->size--;
	if(0 == queue->size){
		queue->head = NULL;
		queue->tail = NULL;
	}
	return job;
}
