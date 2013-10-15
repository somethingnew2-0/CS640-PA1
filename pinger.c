/* The pinger sends a specified series of packets via UDP to the reflector */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include "host.h"
#include "utility.h"

int main(int argc, char * argv[]) {
  char* pingerPort = NULL;
  char* hostname = NULL;
  char* reflectorPort = NULL;
  char* numPacketsStr = NULL;
  int numPackets = -1;

  int c;

  while((c = getopt(argc, argv, "p:s:r:n:")) != -1) {
    switch (c) {
    case 'p':
      pingerPort = optarg;
      break;
    case 's':
      hostname = optarg;
      break;
    case 'r':
      reflectorPort = optarg;
      break;
    case 'n':
      numPacketsStr = optarg;
      break;
    case '?':
      printUsage();
      return 1;
    default:
      exit(EXIT_FAILURE);
    }
  }
  
  checkFlagPresent('p', pingerPort);
  checkFlagPresent('s', hostname);
  checkFlagPresent('r', reflectorPort);
  checkFlagPresent('n', numPacketsStr);

  return 0;
}

void printUsage() {
  printf("Usage: pinger -p <pinger port> -s <hostname> -r <reflector port> -n <number of pac\
kets>\n");
}
