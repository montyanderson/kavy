#ifndef KAVY_CLIENT
#define KAVY_CLIENT

#include "main.h"
#include "buffer.h"

typedef struct Client_s {
	int fd;
	Buffer input;
	Buffer output;
	struct sockaddr_in address;
	socklen_t address_length;
	struct Client_s *next;
} Client;

void client_init(Client *);
int client_handle(Client *);

#endif
