/* Functions to create and manipulate packets */
#ifndef PACKET_H_
#define PACKET_H_

#include "utility.h"

struct packet {
  /* The 32 bit sequence. Set so first bit is 1 */
  unsigned long sequence;

  struct timeval timestamp;
};

typedef struct packet Packet;

Packet * createPacket(int num);
void destroyPacket(Packet * packet);
struct timeval getPacketTimeval(Packet * packet);
long getPacketTimestamp(Packet * packet);
unsigned long getPacketSequence(Packet * packet);

#endif /* Utility_H_ */
