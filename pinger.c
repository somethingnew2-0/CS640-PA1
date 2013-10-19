/* The pinger sends a specified series of packets via UDP to the reflector */

#include <ctype.h>
#include <unistd.h>
#include "host.h"
#include "utility.h"
#include "udp.h"
#include "packet.h"

int main(int argc, char * argv[]) {
  char* pingerPortStr = NULL;
  char* hostname = NULL;
  char* reflectorPortStr = NULL;
  char* numPacketsStr = NULL;
  int pingerPort;
  int reflectorPort;
  int numPackets;
  
  int c;
  while((c = getopt(argc, argv, "p:s:r:n:")) != -1) {
    switch (c) {
    case 'p':
      pingerPortStr = optarg;
      break;
    case 's':
      hostname = optarg;
      break;
    case 'r':
      reflectorPortStr = optarg;
      break;
    case 'n':
      numPacketsStr = optarg;
      break;
    default:
      printUsage();
      return 1;
    }
  }
  
  checkFlagPresent('p', pingerPortStr);
  checkFlagPresent('s', hostname);
  checkFlagPresent('r', reflectorPortStr);
  checkFlagPresent('n', numPacketsStr);
  pingerPort = atoi(pingerPortStr);
  reflectorPort = atoi(reflectorPortStr);
  numPackets = atoi(numPacketsStr);

  int fd;
  if((fd = UDP_Open(pingerPort)) <= 0) {
    printf("UDP_Open error\n");
    return 1;
  }

  SockAddr* reflectorAddr =  (SockAddr*)malloc(sizeof(SockAddr));
  printf("Open socket\n");
  if(UDP_FillSockAddr(reflectorAddr, hostname, reflectorPort) != 0) {
    printf("UDP_Fill error\n");
    return 1;
  }

  for (int i = 0; i < numPackets; i++) {
    Packet* packet = createPacket(i);
    if(UDP_Write(fd, reflectorAddr, packet, sizeof(Packet)) < 0) {
      printf("Send error\n");
      return 1;
    }
    printf("Packet sent from pinger\n");
    destroyPacket(packet);
  }

  for (int i = 0; i < numPackets; i++) {
    Packet * packet = (Packet*)malloc(sizeof(Packet));
    if(UDP_Read(fd, reflectorAddr, packet, sizeof(Packet)) < 0) {
      printf("Read error\n");
      return 1;
    }
    printf("Packet received from reflector %lu\n", packet->timestamp);
    destroyPacket(packet);
  }
  UDP_Close(fd);

  return 0;
}

void printUsage() {
  printf("Usage: pinger -p <pinger port> -s <hostname> -r <reflector port> -n <number of pac\
kets>\n");
}
