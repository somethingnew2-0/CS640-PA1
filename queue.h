#include "utility.h"
#include "packet.h"

typedef struct queue Queue;

Queue * allocate();
void deallocate(Queue *queue);
int enqueue(Queue *queue, QueuedPacket * packet);
QueuedPacket * dequeue(Queue *queue);
QueuedPacket * peek(Queue *queue);