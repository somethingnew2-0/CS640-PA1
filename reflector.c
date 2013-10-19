/* The reflector receives packets from the pinger and either returns 
 * them via UDP to the pinger or drops them.
 */
#include "host.h"
#include "utility.h"
#include "udp.h"
#include "packet.h"

bool dropPacket(int lossProb) {
  return (rand() % 100 + 1) <= lossProb;
}

int main(int argc, char *argv[]) {
  char* reflectorPortStr = NULL;
  char* hostname = NULL;
  char* pingerPortStr = NULL;
  char* delayStr = NULL;
  char* lossProbStr = NULL;
  int reflectorPort;
  int pingerPort;
  int delay;
  int lossProb;

  int c;
  while((c = getopt(argc, argv, "p:s:g:d:l:")) != -1) {
    switch (c) {
    case 'p':
      reflectorPortStr = optarg;
      break;
    case 's':
      hostname = optarg;
      break;
    case 'g':
      pingerPortStr = optarg;
      break;
    case 'd':
      delayStr = optarg;
      break;
    case 'l':
      lossProbStr = optarg;
      break;
    default:
      printUsage();
      return 1;
    }
  }
  
  checkFlagPresent('p', reflectorPortStr);
  checkFlagPresent('s', hostname);
  checkFlagPresent('g', pingerPortStr);
  reflectorPort = atoi(reflectorPortStr);
  pingerPort = atoi(pingerPortStr);
  delay = (delayStr == NULL) ? 0 : atoi(delayStr);
  lossProb = (lossProbStr == NULL) ? 0 : atoi(lossProbStr);

  /* initialize random seed: */
  /*srand(time(NULL));*/
  int fd;
  if((fd = UDP_Open(reflectorPort)) <= 0) {
    printf("UDP_Open error\n");
    return 1;
  }
  
  SockAddr* pingerAddr = (SockAddr*)malloc(sizeof(SockAddr));
  printf("Open socket\n");
  if(UDP_FillSockAddr(pingerAddr, hostname, pingerPort) != 0) {
    printf("UDP_Fill error\n");
    return 1;
  }

  Packet * packet = (Packet*)malloc(sizeof(Packet));
  if(UDP_Read(fd, pingerAddr, packet, sizeof(Packet)) < 0) {
    printf("Read error\n");
    return 1;
  }
  printf("Packet received %lu\n", packet->timestamp);
  
  UDP_Close(fd);
  return 0;
}

void printUsage() {
  printf("Usage: reflector -p <reflector port> -s <hostname> -g <pinger port> -d <delay> -l \
<loss probability>\n");
}
