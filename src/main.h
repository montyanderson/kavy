#ifndef KAVY_MAIN
#define KAVY_MAIN

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <endian.h>
#include <fcntl.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

void error(char *func);

#endif
