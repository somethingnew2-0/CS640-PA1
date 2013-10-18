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

  if(gettimeofday(&currentTime, NULL) < 0) {
    printf("gettimeofday error");
    exit(EXIT_FAILURE);
  }
  packet->sequence = 2147483648L + num;
  /* Need to convert tv_sec to microseconds and add the remaining usec. */
  packet->timestamp = currentTime.tv_sec*1000000 + currentTime.tv_usec;
  
  return packet;
}

void destroyPacket(Packet * packet) {
  free(packet);
}
