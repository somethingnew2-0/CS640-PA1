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
