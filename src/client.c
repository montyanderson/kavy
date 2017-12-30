#include "main.h"
#include "client.h"

const size_t recv_buffer_size = 1;
char *recv_buffer;

void _client_cmd_set(Client *client) {
	const size_t header_length = sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t);

	if(client->input.length < header_length)
		return;

	printf("set handler\n");

	uint32_t key_length;
	buffer_slice(&client->input, sizeof(uint8_t), &key_length, sizeof(uint32_t));
	key_length = be32toh(key_length);

	char *key =	client->input.data + header_length;

	uint32_t value_length;
	buffer_slice(&client->input, sizeof(uint8_t) + sizeof(uint32_t), &value_length, sizeof(uint32_t));
	value_length = be32toh(value_length);

	char *value = client->input.data + header_length + key_length;

	const size_t data_length = key_length + value_length;

	if(client->input.length < header_length + data_length)
		return;

	printf("key: %.*s (%u bytes)\nvalue: %.*s (%u bytes)\n", (int) key_length, key, key_length, (int) value_length, value, value_length);

	buffer_ltrim(&client->input, header_length + value_length);
}

void _client_cmd_get(Client *client) {

}

void (*cmds[])(Client *client) = {
	NULL,
	&_client_cmd_set,
	&_client_cmd_get
};

void client_init(Client *client) {
	buffer_init(&client->input);
	buffer_init(&client->output);
}

void client_free(Client *client) {
	buffer_free(&client->input);
	buffer_free(&client->output);
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
	if(recv_buffer_length < 0) {
		client_free(client);
		return 0;
	}

	if(recv_buffer_length == 0)
		return 1;

	printf("a %d\n", client->input.data[0]);
	// append recv_buffer to client input buffer
	buffer_append(&client->input, recv_buffer, recv_buffer_length);
	// run command handler

	printf("a %d\n", client->input.data[0]);
	cmds[(size_t) client->input.data[0]](client);
	printf("b\n");

	printf("yo\n");

	return 1; // client is still connected
}
