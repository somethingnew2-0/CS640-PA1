/* The pinger sends a specified series of packets via UDP to the reflector */

#include <stdio.h>
#include <stdlib.h>
#include "host.h"
#include "utility.h"

int main(int argc, char * argv[]) {
  char* pingerPort;
  char* hostname;
  char* reflectorPort;
  char* numPacketsStr;
  int numPackets;

  if(argc == 9) {
    pingerPort  = getFlagValue("-p", argc, argv, true);
    if(pingerPort == NULL) { return 0; }
    hostname  = getFlagValue("-s", argc, argv, true);
    if(hostname == NULL) { return 0; }
    reflectorPort  = getFlagValue("-r", argc, argv, true);
    if(reflectorPort == NULL) { return 0; }
    numPacketsStr  = getFlagValue("-n", argc, argv, true);
    if(numPacketsStr == NULL) { return 0; }
    
    numPackets = atoi(numPacketsStr);
  }
  else {
    printUsage();
    return 0;
  }

  return 0;
}

void printUsage() {
  printf("Usage: pinger -p <pinger port> -s <hostname> -r <reflector port> -n <number of pac\
kets>\n");
}
