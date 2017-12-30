#ifndef KAVY_SERVER
#define KAVY_SERVER

#include "main.h"
#include "client.h"
#include "dict.h"

typedef struct {
	int fd;
	Client *client;
	Dict dict;
	char *recv_buffer;
	size_t recv_buffer_size;
} Server;

void server_init(Server *);
void server_tick(Server *);

#endif
