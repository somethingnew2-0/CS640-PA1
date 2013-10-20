/* The pinger sends a specified series of packets via UDP to the reflector */

#include <ctype.h>
#include <unistd.h>
#include "host.h"
#include "utility.h"
#include "udp.h"
#include "packet.h"

void printAndRecordPacketInfo(Packet* packet, SockAddr* sockAddr, long* minRTT, 
			      long* maxRTT, long* totalRTT) {
  long rtt = getTimestamp() - packet->timestamp;
  *totalRTT += rtt;
  if(rtt < *minRTT && *minRTT >= 0) {
    *minRTT = rtt;
  }
  if(rtt > *maxRTT) {
    *maxRTT = rtt;
  }

  printf("Packet received from reflector\n");
  printf("Size: %i\n", sizeof(Packet));
  printf("Reflector IP: %s\n", formatIP((sockAddr->sin_addr).s_addr));
  printf("RTT: %lu\n", rtt);
}

int main(int argc, char * argv[]) {
  char* pingerPortStr = NULL;
  char* hostname = NULL;
  char* reflectorPortStr = NULL;
  char* numPacketsStr = NULL;
  int pingerPort;
  int reflectorPort;
  int numPackets;
 
  long minRTT = -1;
  long maxRTT = -1;
  long totalRTT = 0;

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
  struct timeval timeout = {1, 0};
  Packet* recvPacket = (Packet*)malloc(sizeof(Packet));
  while(packetsSent < numPackets || packetsRecieved < numPackets) {
    if(packetsSent < numPackets) {
      fd_set fds;
      FD_ZERO(&fds);
      FD_SET(fd, &fds);
      
      /* Create a descriptor set containing our two sockets.  */
      int rc = select(fd+1, &fds, NULL, NULL, &timeout);
            
      /* select error */
      if(rc < 0) {
        return -1;
      }
      /* No data since timeout */        
      else if (rc == 0) {
	/* Reset the time limit and send the packet */
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
	Packet* sendPacket = createPacket(packetsSent++);
	if(UDP_Write(fd, reflectorAddr, sendPacket, sizeof(Packet)) < 0) {
	  printf("Send error\n");
	  return 1;
	}
	destroyPacket(sendPacket);
        printf("Packet sent from pinger\n");
      }
      /* Data is available */
      else {
	if(UDP_Read(fd, reflectorAddr, recvPacket, sizeof(Packet)) < 0) {
	  printf("Read error\n");
	  return 1;
	}

        printAndRecordPacketInfo(recvPacket, reflectorAddr, &minRTT, &maxRTT, &totalRTT);
        packetsRecieved++;
      }
    }
    else {
      if(UDP_Read(fd, reflectorAddr, recvPacket, sizeof(Packet)) < 0) {
	printf("Read error\n");
	return 1;
      }

      printAndRecordPacketInfo(recvPacket, reflectorAddr, &minRTT, &maxRTT, &totalRTT);
      packetsRecieved++;
    }
  }
  destroyPacket(recvPacket);
  UDP_Close(fd);

  printf("\nPackets sent: %d\n", numPackets);
  printf("Packets received: %d\n", packetsRecieved);
  printf("Loss percentage: %f\n", 1 - ((double)packetsRecieved)/numPackets);
  printf("RTT min/avg/max: %lu/%lu/%lu\n", minRTT/1000, 
	 totalRTT/(packetsRecieved*1000), maxRTT/1000);

  return 0;
}

void printUsage() {
  printf("Usage: pinger -p <pinger port> -s <hostname> -r <reflector port> -n <number of pac\
kets>\n");
}
