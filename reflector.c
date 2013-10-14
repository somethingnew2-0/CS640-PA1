/* The reflector receives packets from the pinger and either returns 
 * them via UDP to the pinger or drops them.
 */
#include <stdio.h>
#include <stdlib.h>
#include "host.h"
#include "utility.h"

int main(int argc, char *argv[]) {
  char* reflectorPort;
  char* hostname;
  char* pingerPort;
  char* delayStr;
  char* lossProbStr;
  int delay;
  int lossProb;

  if(argc == 11 || argc == 9 || argc == 7) {
    reflectorPort = getFlagValue("-p", argc, argv, true);
    if(reflectorPort == NULL) { return 0; }
    hostname = getFlagValue("-s", argc, argv, true);
    if(hostname == NULL) { return 0; }
    pingerPort = getFlagValue("-g", argc, argv, true);
    if(pingerPort == NULL) { return 0; }
    delayStr = getFlagValue("-d", argc, argv, false);
    lossProbStr = getFlagValue("-l", argc, argv, false);
    
    /* If delay or loss probability are not specified set them to 0 */
    delay = (delayStr == NULL) ? 0 : atoi(delayStr);
    lossProb = (lossProbStr == NULL) ? 0 : atoi(lossProbStr);
  }
  else {
    printUsage();
    return 0;
  }

  return 0;
}

void printUsage() {
  printf("Usage: reflector -p <reflector port> -s <hostname> -g <pinger port> -d <delay> -l \
<loss probability>\n");
}
