/* Functions to create and manipulate packets */
#ifndef PACKET_H_
#define PACKET_H_

#include <stdint.h>
#include "utility.h"
#include "udp.h"

typedef struct packet {
  /* The 32 bit sequence. Set so first bit is 1 */
  uint32_t sequence;

  /* Time in microseconds */
  long timestamp;
} Packet;

typedef struct queuedpacket
{
  Packet* packet;

  /* Time in microseconds this packet was queued */
  long timestamp;
  
  uint32_t ipAddress;
} QueuedPacket;


Packet* createPacket(int num);
QueuedPacket* createQueuedPacket(Packet* packet, SockAddr* sockaddr);
void destroyQueuedPacket(QueuedPacket * packet);
void destroyPacket(Packet * packet);

#endif
