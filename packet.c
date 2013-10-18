/* Functions to create and manipulate packets */

#include "packet.h"

/* Creates a new packet at the current time */
Packet * createPacket(int num) {
  Packet * packet;
  struct timeval currentTime;
  
  if(num < 0) {
    return NULL;
  }
  
  packet = (Packet *)malloc(sizeof(Packet));
  if(packet == NULL) {
    printf("malloc error");
    exit(EXIT_FAILURE);
  }

  gettimeofday(&currentTime, NULL);
  packet->sequence = 2147483648L + num;
  packet->timestamp = currentTime;
  
  return packet;
}

void destroyPacket(Packet * packet) {
  free(packet);
}

struct timeval getPacketTimeval(Packet * packet) {
  return packet->timestamp;
}

long getPacketTimestamp(Packet * packet) {
  return (long)((packet->timestamp).tv_usec);
}

unsigned long getPacketSequence(Packet * packet) {
  return packet->sequence;
}
