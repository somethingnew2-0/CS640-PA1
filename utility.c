/* Useful functions for the pinger and reflector */

#include "utility.h"

void checkFlagPresent(char flag, char* value) {
  if(value == NULL) {
    printUsage();
    printf("Missing -%c option.\n", flag);
    exit(EXIT_FAILURE);
  }
}
