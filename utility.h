/* Useful functions for pinger and reflector */
#ifndef UTILITY_H_
#define UTILITY_H_

#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#include <netdb.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "host.h"

void checkFlagPresent(char flag, char* value);
long getTimestamp();
char* formatIP(uint32_t ip);
void checkMallocError(void * object);

#endif /* UTILITY_H_ */
