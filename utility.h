/* Useful functions for pinger and reflector */
#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "host.h"

char* getFlagValue(char* flag, int argc, char* argv[], bool required);

#endif /* UTILITY_H_ */
