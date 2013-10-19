/* Functions to create and manipulate packets */

#include "packet.h"

/* Creates a new packet at the current time */
Packet* createPacket(int num) {
  if(num < 0) {
    printf("num less than 0 error");
    return NULL;
  }
  
  Packet* packet = (Packet *)malloc(sizeof(Packet));
  if(packet == NULL) {
    printf("malloc error");
    exit(EXIT_FAILURE);
  }

  packet->sequence = 2147483648L + num;
  
  packet->timestamp = getTimestamp();
  
  return packet;
}

/* Creates a new queued packet at the current time */
QueuedPacket* createQueuedPacket(Packet* packet) {
  if(packet == NULL) {
    printf("null packet error");
    return NULL;
  }

  QueuedPacket* queuedPacket = (QueuedPacket *)malloc(sizeof(QueuedPacket));
  queuedPacket->packet = packet;
  queuedPacket->timestamp = getTimestamp();

  return queuedPacket;
}

void destroyQueuedPacket(QueuedPacket* packet) {
  destroyPacket(packet->packet);
  free(packet);
}

void destroyPacket(Packet* packet) {
  free(packet);
}
