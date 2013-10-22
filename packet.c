/* Functions to create and manipulate packets */
#include "packet.h"

/* Creates a new packet at the current time */
Packet* createPacket(int num) {
  if(num < 0) {
    printf("packet num less than 0 error");
    return NULL;
  }
  
  Packet* packet = (Packet *)malloc(sizeof(Packet));
  checkMallocError(packet);

  /* set the sequence and timestamp */
  packet->sequence = htonl(2000000000L + num);
  packet->timestamp = getTimestamp();
  
  return packet;
}

/* Creates a new queued packet at the current time */
QueuedPacket* createQueuedPacket(Packet* packet, SockAddr* sockAddr) {
  if(packet == NULL || sockAddr == NULL) {
    printf("queuedpacket null param error");
    return NULL;
  }

  QueuedPacket* queuedPacket = (QueuedPacket *)malloc(sizeof(QueuedPacket));
  checkMallocError(queuedPacket);

  queuedPacket->packet = packet;
  queuedPacket->timestamp = getTimestamp();
  queuedPacket->ipAddress = (sockAddr->sin_addr).s_addr;

  return queuedPacket;
}

void destroyQueuedPacket(QueuedPacket* packet) {
  destroyPacket(packet->packet);
  free(packet);
}

void destroyPacket(Packet* packet) {
  free(packet);
}
