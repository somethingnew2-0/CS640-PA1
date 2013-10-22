/* Functions used to setup a udp connection, send, and receive packets */
#ifndef UDP_H_
#define UDP_H_

#include "utility.h"

typedef struct sockaddr_in SockAddr;

int udpOpen(int port);
int udpClose(int fd);

int udpRead(int fd, SockAddr *addr, void *buffer, int n);
int udpWrite(int fd, SockAddr *addr, void *buffer, int n);

int udpFillSockAddr(SockAddr *addr, char *hostName, int port);

#endif /* UDP_H_ */
