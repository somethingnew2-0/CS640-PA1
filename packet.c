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

  struct timeval currentTime;
  if(gettimeofday(&currentTime, NULL) < 0) {
    printf("gettimeofday error");
    exit(EXIT_FAILURE);
  }
  packet->sequence = 2147483648L + num;
  /* Need to convert tv_sec to microseconds and add the remaining usec. */
  packet->timestamp = currentTime.tv_sec*1000000 + currentTime.tv_usec;
  
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

  struct timeval currentTime;
  if(gettimeofday(&currentTime, NULL) < 0) {
    printf("gettimeofday error");
    exit(EXIT_FAILURE);
  }

  /* Need to convert tv_sec to microseconds and add the remaining usec. */
  packet->timestamp = currentTime.tv_sec*1000000 + currentTime.tv_usec;

  return queuedPacket;
}

void destroyPacket(Packet* packet) {
  free(packet);
}
