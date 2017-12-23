#include "main.h"
#include "client.h"

const size_t recv_buffer_size = 1024 * 1024;
char *recv_buffer;

void client_init(Client *client) {

}

int client_handle(Client *client) {
	// if recv_buffer hasn't been allocated yet
	// todo: move to server startup
	if(recv_buffer == NULL) {
		recv_buffer = malloc(recv_buffer_size);

		if(recv_buffer == NULL)
			error("malloc");
	}

	ssize_t recv_buffer_length = recv(client->fd, recv_buffer, recv_buffer_size, 0);

	// check if client still connected
	if(recv_buffer_length <= 0)
		return 0; // disconnected

	// append recv_buffer to client buffer
	// resize
	client->buffer = realloc(client->buffer, client->buffer_length + recv_buffer_length);
	// copy
	memcpy(client->buffer + client->buffer_length, recv_buffer, recv_buffer_length);
	// set new length
	client->buffer_length += recv_buffer_length;

	// write back to client
	send(client->fd, client->buffer, client->buffer_length, 0);

	return 1; // client is still connected
}
