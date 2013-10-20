/* The reflector receives packets from the pinger and either returns 
 * them via UDP to the pinger or drops them.
 */
#include "host.h"
#include "utility.h"
#include "udp.h"
#include "packet.h"
#include "queue.h"

bool dropPacket(int lossProb) {
  return (rand() % 100 + 1) <= lossProb;
}

void printPacketInfo(QueuedPacket * queuedPacket) {
  printf("Packet received from pinger\n");
  printf("Time: %lu\n", queuedPacket->timestamp);
  printf("Pinger IP: %s\n", formatIP(queuedPacket->ipAddress));
  printf("Sequence #: %lu\n", (unsigned long)queuedPacket->packet->sequence);
}

int reflector(int fd, SockAddr* pingerAddr, Queue* queue, int delay, int lossProb) {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  
  struct timeval timeout;
  /* Set time limit. */
  
  if(queue->size > 0) {
    timeout.tv_sec = 0;
    timeout.tv_usec = (delay - (getTimestamp() - peek(queue)->timestamp)) * 1000;
    if(timeout.tv_usec < 0) {
      timeout.tv_usec = 0;
    }
  }
  else {
    timeout.tv_sec = 0;
    timeout.tv_usec = 1500000;
  }
  /* Create a descriptor set containing our two sockets.  */
  int rc = select(fd+1, &fds, NULL, NULL, &timeout);
        
  /* select error */
  if(rc < 0) {
    return -1;
  }
  /* No data since timeout */        
  else if (rc == 0) {
    if(queue->size > 0) {
      QueuedPacket* queuedPacket = dequeue(queue);
      printPacketInfo(queuedPacket);
      if(dropPacket(lossProb)) {
        printf("Packet dropped\n");
      }
      else { 
        if(UDP_Write(fd, pingerAddr, queuedPacket->packet, sizeof(Packet)) < 0) {
          printf("Send error\n");
          return 1;
        }
        printf("Packet not dropped\n");
      }
      destroyQueuedPacket(queuedPacket);
      return reflector(fd, pingerAddr, queue, delay, lossProb);
    }
    else {
      return 0;
    }
  }
  /* Data is available */
  else {
    Packet * packet = (Packet*)malloc(sizeof(Packet));
    if(UDP_Read(fd, pingerAddr, packet, sizeof(Packet)) < 0) {
      printf("Read error\n");
      return 1;
    }
    enqueue(queue, createQueuedPacket(packet, pingerAddr));
    
    return reflector(fd, pingerAddr, queue, delay, lossProb);
  }  
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
  srand((int)getTimestamp());
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

  Queue* queue = allocate();
  // Wait for the first packet indefintely
  Packet * packet = (Packet*)malloc(sizeof(Packet));
  if(UDP_Read(fd, pingerAddr, packet, sizeof(Packet)) < 0) {
    printf("Read error\n");
    return 1;
  }
  enqueue(queue, createQueuedPacket(packet, pingerAddr));

  if(reflector(fd, pingerAddr, queue, delay, lossProb) != 0)  {
    printf("Reflector error\n");
    return 1;
  }

  deallocate(queue);

  UDP_Close(fd);
  return 0;
}

void printUsage() {
  printf("Usage: reflector -p <reflector port> -s <hostname> -g <pinger port> -d <delay> -l \
<loss probability>\n");
}
