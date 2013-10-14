/* Useful functions for the pinger and reflector */

#include "utility.h"

char* getFlagValue(char* flag, int argc, char* argv[], bool required) {
  int i;
  for(i = 1; i < argc; i += 2) {
    if(strcmp(flag, argv[i]) == 0) {
      return argv[i+1];
    }
  }

  if(required) {
    printUsage();
    printf("Missing %s flag\n", flag);
  }
  return NULL;
}
