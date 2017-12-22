#include "main.h"
#include "client.h"

void client_init(Client *client) {
	
}

int client_handle(Client *client) {
	size_t recv_buffer_length = 1024 * 1024;
	char *recv_buffer = malloc(recv_buffer_length);

	if(recv_buffer == NULL)
		error("malloc");

	memset(recv_buffer, 0, recv_buffer_length);

	ssize_t recv_length = recv(client->fd, recv_buffer, recv_buffer_length, 0);

	if(recv_length <= 0)
		return 0;

	printf("data from %d: %s\n", client->fd, recv_buffer);

	free(recv_buffer);

	return 1;
}
