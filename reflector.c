/* The reflector receives packets from the pinger and either returns 
 * them via UDP to the pinger or drops them.
 */
#include "packet.h"
#include "queue.h"
#include "udp.h"
#include "utility.h"

/* Determine if the current packet should be dropped */
bool dropPacket(int lossProb) {
  return (rand() % 100 + 1) <= lossProb;
}

void printPacketInfo(QueuedPacket * queuedPacket) {
  printf("Packet received. Time: %lu, Pinger IP: %s, Sequence #: %lu, ", 
	 queuedPacket->timestamp/1000, formatIP(queuedPacket->ipAddress), 
	 (unsigned long)ntohl(queuedPacket->packet->sequence));
}

int reflector(int fd, SockAddr* pingerAddr, Queue* queue, int delay, int lossProb) {
  fd_set fds;
  FD_ZERO(&fds);
  FD_SET(fd, &fds);
  
  struct timeval timeout;
  int rc;
  if(queue->size > 0) {
    /* Set time limit. */
    timeout.tv_sec = 0;
    timeout.tv_usec = (delay * 1000) - (getTimestamp() - peek(queue)->timestamp);
    if(timeout.tv_usec < 0) {
      timeout.tv_usec = 0;
    }

    /* Create a descriptor set containing our two sockets.  */
    rc = select(fd+1, &fds, NULL, NULL, &timeout);
  }
  else {
    /* Create a descriptor set containing our sockets that runs indefinitely. */
    rc = select(fd+1, &fds, NULL, NULL, NULL);
  }
        
  /* select error */
  if(rc < 0) {
    return -1;
  }
  /* No data since timeout */        
  else if (rc == 0) {
    /* Send a packet to pinger */
    QueuedPacket* queuedPacket = dequeue(queue);
    if(udpWrite(fd, pingerAddr, queuedPacket->packet, sizeof(Packet)) < 0) {
      printf("Send error\n");
      return 1;
    }
    destroyQueuedPacket(queuedPacket);

    return reflector(fd, pingerAddr, queue, delay, lossProb);
  }
  /* Data is available */
  else {
    Packet * packet = (Packet*)malloc(sizeof(Packet));
    checkMallocError(packet);
    if(udpRead(fd, pingerAddr, packet, sizeof(Packet)) < 0) {
      printf("Read error\n");
      return 1;
    }

    QueuedPacket * queuedPacket = createQueuedPacket(packet, pingerAddr);
    printPacketInfo(queuedPacket);
    if(dropPacket(lossProb)) {
      /* If the packet is dropped, just delete it */
      printf("Dropped\n");
      destroyQueuedPacket(queuedPacket);
    }
    else {
      /* else place the packet in the queue */
      printf("Not dropped\n");
      enqueue(queue, queuedPacket);
    }

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

  /* Open the socket */
  int fd;
  if((fd = udpOpen(reflectorPort)) <= 0) {
    return 1;
  }
  printf("Open socket\n");

  /* Fill address information */
  SockAddr* pingerAddr = (SockAddr*)malloc(sizeof(SockAddr));
  checkMallocError(pingerAddr);
  if(udpFillSockAddr(pingerAddr, hostname, pingerPort) != 0) {
    return 1;
  }

  Queue* queue = allocate();
  if(reflector(fd, pingerAddr, queue, delay, lossProb) != 0)  {
    printf("Reflector error\n");
    return 1;
  }

  deallocate(queue);
  udpClose(fd);

  return 0;
}

void printUsage() {
  printf("Usage: reflector -p <reflector port> -s <hostname> -g <pinger port> -d <delay> -l \
<loss probability>\n");
}
