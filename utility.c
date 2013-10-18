/* Useful functions for the pinger and reflector */

#include "utility.h"

struct packet {
  /* The 32 bit sequence. Set so first bit is 1 */
  unsigned long sequence;

  struct timeval timestamp;
};

void checkFlagPresent(char flag, char* value) {
  if(value == NULL) {
    printUsage();
    printf("Missing -%c option.\n", flag);
    exit(EXIT_FAILURE);
  }
}
