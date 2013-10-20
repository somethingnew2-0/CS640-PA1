/* Useful functions for pinger and reflector */
#ifndef UTILITY_H_
#define UTILITY_H_

#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "host.h"

void checkFlagPresent(char flag, char* value);
long getTimestamp();
char* formatIP(uint32_t ip);

#endif /* UTILITY_H_ */
