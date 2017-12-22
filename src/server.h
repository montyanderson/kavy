#ifndef KAVY_SERVER
#define KAVY_SERVER

#include "main.h"

typedef struct {
	int fd;
} Server;

void server_init(Server *);
void server_tick(Server *, Client **);

#endif
