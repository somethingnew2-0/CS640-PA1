/* The reflector receives packets from the pinger and either returns 
 * them via UDP to the pinger or drops them.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "host.h"
#include "utility.h"

bool dropPacket(int lossProb) {
  return (rand() % 100 + 1) <= lossProb;
}

int main(int argc, char *argv[]) {
  char* reflectorPort = NULL;
  char* hostname = NULL;
  char* pingerPort = NULL;
  char* delayStr = NULL;
  char* lossProbStr = NULL;
  int delay;
  int lossProb;
  int c;

  while((c = getopt(argc, argv, "p:s:g:d:l:")) != -1) {
    switch (c) {
    case 'p':
      reflectorPort = optarg;
      break;
    case 's':
      hostname = optarg;
      break;
    case 'g':
      pingerPort = optarg;
      break;
    case 'd':
      delayStr = optarg;
      break;
    case 'l':
      lossProbStr = optarg;
      break;
    case '?':
      printUsage();
      return 1;
    default:
      exit(EXIT_FAILURE);
    }
  }
  
  checkFlagPresent('p', reflectorPort);
  checkFlagPresent('s', hostname);
  checkFlagPresent('g', pingerPort);
  delay = (delayStr == NULL) ? 0 : atoi(delayStr);
  lossProb = (lossProbStr == NULL) ? 0 : atoi(lossProbStr);

  /* initialize random seed: */
  srand(time(NULL));


  return 0;
}

void printUsage() {
  printf("Usage: reflector -p <reflector port> -s <hostname> -g <pinger port> -d <delay> -l \
<loss probability>\n");
}
