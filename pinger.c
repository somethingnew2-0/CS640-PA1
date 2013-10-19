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



  int packetsSent = 0, packetsRecieved = 0;
  while(packetsSent < numPackets && packetsRecieved < numPackets) {
    if(packetsSent < numPackets) {
      fd_set fds;
      FD_ZERO(&fds);
      FD_SET(fd, &fds);
      
      /* Set time limit. */
      struct timeval timeout;
      timeout.tv_sec = 1;
      timeout.tv_usec = 0;
      
      /* Create a descriptor set containing our two sockets.  */
      int rc = select(fd+1, &fds, NULL, NULL, &timeout);
            
      /* select error */
      if(rc < 0) {
        return -1;
      }
      /* No data since timeout */        
      else if (rc == 0) {
        Packet* packet = createPacket(packetsSent++);
        if(UDP_Write(fd, reflectorAddr, packet, sizeof(Packet)) < 0) {
          printf("Send error\n");
          return 1;
        }
        printf("Packet sent from pinger\n");
        destroyPacket(packet);
      }
      /* Data is available */
      else {
        Packet * packet = (Packet*)malloc(sizeof(Packet));
        if(UDP_Read(fd, reflectorAddr, packet, sizeof(Packet)) < 0) {
          printf("Read error\n");
          return 1;
        }
        printf("Packet received from reflector %lu\n", packet->timestamp);
        destroyPacket(packet);
        packetsRecieved++;
      }
    }
    else {
        Packet * packet = (Packet*)malloc(sizeof(Packet));
        if(UDP_Read(fd, reflectorAddr, packet, sizeof(Packet)) < 0) {
          printf("Read error\n");
          return 1;
        }
        printf("Packet received from reflector %lu\n", packet->timestamp);
        destroyPacket(packet);
        packetsRecieved++;
    }
  }
  UDP_Close(fd);

  return 0;
}

void printUsage() {
  printf("Usage: pinger -p <pinger port> -s <hostname> -r <reflector port> -n <number of pac\
kets>\n");
}
