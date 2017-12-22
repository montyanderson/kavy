#ifndef KAVY_CLIENT
#define KAVY_CLIENT

#include "main.h"

typedef struct Client_s {
	int fd;
	char *buffer;
	size_t buffer_length;
	size_t buffer_size;
	struct sockaddr_in address;
	socklen_t address_length;
	struct Client_s *next;
} Client;

void client_init(Client *);
int client_handle(Client *);

#endif
