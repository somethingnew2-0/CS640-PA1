/* A queue implementation for reflector */
#ifndef QUEUE_H_
#define QUEUE_H_

#include "packet.h"
#include "utility.h"

/* A node in a linked list */
typedef struct node {
  /* a Packet of the node */
  QueuedPacket* packet;
  
  /* the next node in the list */
  struct node* next;
} Node;

typedef struct queue {
  /* the size of the queue */
  int size;

  /* a pointer to the head of the queue */
  Node* head;

  /* a pointer to the tail of the queue */
  Node* tail;
} Queue;

Queue * allocate();
void deallocate(Queue *queue);
int enqueue(Queue* queue, QueuedPacket* packet);
QueuedPacket * dequeue(Queue *queue);
QueuedPacket * peek(Queue *queue);

#endif /* QUEUE_H_ */
