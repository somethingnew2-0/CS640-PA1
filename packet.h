/* Functions to create and manipulate packets */
#ifndef PACKET_H_
#define PACKET_H_

#include "udp.h"
#include "utility.h"

typedef struct packet {
  /* The 32 bit sequence */
  uint32_t sequence;

  /* Time in microseconds */
  long timestamp;
} __attribute__((__packed__)) Packet;

typedef struct queuedpacket {
  /* The packet received from the pinger */
  Packet* packet;

  /* Time in microseconds this packet was queued */
  long timestamp;
  
  /* The IP address of the pinger who sent the packet */
  uint32_t ipAddress;
} QueuedPacket;

Packet* createPacket(int num);
QueuedPacket* createQueuedPacket(Packet* packet, SockAddr* sockaddr);
void destroyQueuedPacket(QueuedPacket * packet);
void destroyPacket(Packet * packet);

#endif /* PACKET_H_ */
