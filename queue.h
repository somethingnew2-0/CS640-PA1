#include "utility.h"
#include "packet.h"

typedef struct queue Queue;

Queue * allocate();
void deallocate(Queue *queue);
int enqueue(Queue *queue, Packet * packet);
Packet * dequeue(Queue *queue);