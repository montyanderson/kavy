#include "main.h"
#include "client.h"
#include "dict.h"

int _client_cmd_set(Client *client, Dict *dict) {
	const size_t header_length = sizeof(uint8_t) + sizeof(uint32_t) + sizeof(uint32_t);

	if(client->input.length < header_length)
		return 0;

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
		return 0;

	dict_set(dict, key, key_length, value, value_length);

	buffer_ltrim(&client->input, header_length + data_length);

	char result = 1;

	buffer_append(&client->output, &result, 1);

	return 1;
}

int _client_cmd_get(Client *client, Dict *dict) {
	const size_t header_length = sizeof(uint8_t) + sizeof(uint32_t);

	if(client->input.length < header_length)
		return 0;

	uint32_t key_length;
	buffer_slice(&client->input, sizeof(uint8_t), &key_length, sizeof(uint32_t));
	key_length = be32toh(key_length);

	char *key = client->input.data + header_length;

	if(client->input.length < header_length + key_length)
		return 0;

	char *value;
	size_t value_length;

	if(dict_get(dict, key, key_length, &value, &value_length)) {
		char result = 2;
		uint32_t value_length_reply = htobe32((uint32_t) value_length);

		buffer_append(&client->output, &result, 1);
		buffer_append(&client->output, (char *) &value_length_reply, sizeof(uint32_t));
		buffer_append(&client->output, value, value_length);
	} else {
		char result = 0;
		buffer_append(&client->output, &result, 1);
	}

	buffer_ltrim(&client->input, header_length + key_length);

	return 1;
}

int (*cmds[])(Client *, Dict *) = {
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

int client_handle(Client *client, Dict *dict, char *recv_buffer, size_t recv_buffer_size) {
	ssize_t recv_buffer_length = recv(client->fd, recv_buffer, recv_buffer_size, 0);

	// check if client still connected
	if(recv_buffer_length <= 0)
		return 0;

	// append recv_buffer to client input buffer
	buffer_append(&client->input, recv_buffer, recv_buffer_length);
	// run command handler

	if(client->input.data[0] >= sizeof(cmds) / sizeof(cmds[0])) {
		return 1;
	}

	while(cmds[(size_t) client->input.data[0]](client, dict) && client->input.length > 0) {
		;
	}

	return 1; // client is still connected
}
