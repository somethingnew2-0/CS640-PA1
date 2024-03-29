/* The pinger sends a specified series of packets via UDP to the reflector */

#include "packet.h"
#include "udp.h"
#include "utility.h"

void printAndRecordPacketInfo(Packet* packet, SockAddr* sockAddr, double* minRTT,
			      double* maxRTT, double* totalRTT) {
  double rtt = (getTimestamp() - packet->timestamp)/1000.0;
  *totalRTT += rtt;
  if(rtt < *minRTT || *minRTT < 0) {
    *minRTT = rtt;
  }
  if(rtt > *maxRTT) {
    *maxRTT = rtt;
  }

  printf("Packet received. Size: %lu, Reflector IP: %s, RTT: %.3fms\n", 
	 sizeof(Packet), formatIP((sockAddr->sin_addr).s_addr), rtt);
}

int main(int argc, char * argv[]) {
  char* pingerPortStr = NULL;
  char* hostname = NULL;
  char* reflectorPortStr = NULL;
  char* numPacketsStr = NULL;
  int pingerPort;
  int reflectorPort;
  int numPackets;
 
  double minRTT = -1;
  double maxRTT = -1;
  double totalRTT = 0;

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

  if(pingerPort < 1024 || pingerPort > 65536 || reflectorPort < 1024 || 
     reflectorPort > 65536) {
    printf("Invalid Port number\n");
    return 1;
  }

  /* Open the socket */
  int fd;
  if((fd = udpOpen(pingerPort)) <= 0) {
    return 1;
  }
  printf("Open socket\n");

  /* Fill the socket address */
  SockAddr* reflectorAddr = (SockAddr*)malloc(sizeof(SockAddr));
  checkMallocError(reflectorAddr);
  if(udpFillSockAddr(reflectorAddr, hostname, reflectorPort) != 0) {
    return 1;
  }

  /* Set up a timeout for 1 second */
  struct timeval timeout = {1, 0};
  int packetsSent = 0, packetsRecieved = 0;
  bool finished = false;
  Packet* recvPacket = (Packet*)malloc(sizeof(Packet));
  checkMallocError(recvPacket);

  while(!finished) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    
    /* Create a descriptor set containing our two sockets.  */
    int rc = select(fd+1, &fds, NULL, NULL, &timeout);
    
    /* select error */
    if(rc < 0) {
      return -1;
    }
    /* Timeout has ended and the program is quiting */
    else if(rc == 0 && packetsSent >= numPackets) {
      finished = true;
    }
    /* No data since timeout */        
    else if (rc == 0) {
      /* Send the packet */
      Packet* sendPacket = createPacket(packetsSent++);
      if(udpWrite(fd, reflectorAddr, sendPacket, sizeof(Packet)) < 0) {
	printf("Send error\n");
	return 1;
      }
      destroyPacket(sendPacket);
      printf("Packet sent from pinger\n");

      /* Reset the timeout */
      timeout.tv_sec = (packetsSent < numPackets) ? 1 : 3;
      timeout.tv_usec = 0;
    }
    /* Data is available */
    else {
      /* Receive a new packet */
      if(udpRead(fd, reflectorAddr, recvPacket, sizeof(Packet)) < 0) {
	printf("Read error\n");
	return 1;
      }
      printAndRecordPacketInfo(recvPacket, reflectorAddr, &minRTT, &maxRTT, 
			       &totalRTT);
      packetsRecieved++;
      if(packetsSent >= numPackets) {
	/* Reset the timeout */
	timeout.tv_sec = 3;
	timeout.tv_usec = 0;
      }
    }
  }

  destroyPacket(recvPacket);
  udpClose(fd);

  printf("\nPackets sent: %d, received: %d, Loss %%: %.3f, RTT min/avg/max: %.3fms/%.3fms/%.3fms\n", 
	 packetsSent, packetsRecieved, 1 - ((double)packetsRecieved)/packetsSent, 
	 minRTT, totalRTT/packetsRecieved, maxRTT);

  return 0;
}

void printUsage() {
  printf("Usage: pinger -p <pinger port> -s <hostname> -r <reflector port> -n <number of pac\
kets>\n");
}
