/* Functions to create and manipulate packets */
#ifndef PACKET_H_
#define PACKET_H_

#include <stdint.h>
#include "utility.h"

struct packet {
  /* The 32 bit sequence. Set so first bit is 1 */
  uint32_t sequence;

  /* Time in microseconds */
  long timestamp;
};

typedef struct packet Packet;

struct queuedpacket
{
	Packet* packet;

	/* Time in microseconds this packet was queued */
	long timestamp;
};

typedef struct queuedpacket QueuedPacket;


Packet* createPacket(int num);
QueuedPacket* createQueuedPacket(Packet* packet);
void destroyPacket(Packet * packet);

#endif