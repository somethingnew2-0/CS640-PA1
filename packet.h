/* Functions to create and manipulate packets */
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
