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

Packet* createPacket(int num);
void destroyPacket(Packet * packet);

#endif