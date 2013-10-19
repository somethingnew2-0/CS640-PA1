/* Functions to create and manipulate packets */
#ifndef PACKET_H_
#define PACKET_H_

#include <stdint.h>
#include "utility.h"

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
} QueuedPacket;


Packet* createPacket(int num);
QueuedPacket* createQueuedPacket(Packet* packet);
void destroyQueuedPacket(QueuedPacket * packet);
void destroyPacket(Packet * packet);

#endif