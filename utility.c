/* Useful functions for the pinger and reflector */

#include "utility.h"

void checkFlagPresent(char flag, char* value) {
  if(value == NULL) {
    printUsage();
    printf("Missing -%c option.\n", flag);
    exit(EXIT_FAILURE);
  }
}

long getTimestamp() {
  struct timeval currentTime;
  if(gettimeofday(&currentTime, NULL) < 0) {
    printf("gettimeofday error");
    exit(EXIT_FAILURE);
  }

  /* Need to convert tv_sec to microseconds and add the remaining usec. */
  return currentTime.tv_sec*1000000 + currentTime.tv_usec;
}

char* formatIP(uint32_t ip) {
  unsigned char bytes[4];
  bytes[0] = ip & 0xFF;
  bytes[1] = (ip >> 8) & 0xFF;
  bytes[2] = (ip >> 16) & 0xFF;
  bytes[3] = (ip >> 24) & 0xFF;
  char* string = malloc(sizeof(char)*16);
  checkMallocError(string);
  sprintf(string, "%d.%d.%d.%d", bytes[0], bytes[1], bytes[2], bytes[3]);
  return string;
}

void checkMallocError(void * object) {
  if(object == NULL) {
    printf("malloc error");
    exit(EXIT_FAILURE);
  }
}
