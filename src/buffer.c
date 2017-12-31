#include "main.h"
#include "buffer.h"

void buffer_init(Buffer *buffer) {
	buffer->length = 0;
	buffer->container = 1;
	buffer->data = malloc(buffer->container);

	if(buffer->data == NULL)
		error("malloc");
}

void buffer_free(Buffer *buffer) {
	free(buffer->data);
}

void buffer_ltrim(Buffer *buffer, size_t n) {
	if(buffer->length == n) {
		buffer->length = 0;
		return;
	}

	buffer->length -= n;
	memmove(buffer->data, buffer->data + n, buffer->length);

	size_t container = buffer->container;

	while(container >= buffer->length) {
		container /= 2;
	}

	container *= 2;

	if(buffer->container > container) {
		buffer->data = realloc(buffer->data, buffer->container = container);

		if(buffer->data == NULL)
			error("realloc");
	}
}

void buffer_append(Buffer *buffer, char *data, size_t data_length) {
	//printf("%lu\n", buffer->length);

	const size_t index = buffer->length;
	buffer->length += data_length;

	size_t container = buffer->container;

	while(container < buffer->length) {
		container *= 2;
	}

	if(buffer->container != container) {
		buffer->data = realloc(buffer->data, buffer->container = container);
	}

	memcpy(buffer->data + index, data, data_length);
}

void buffer_slice(Buffer *buffer, size_t index, void *dest, size_t length) {
	memcpy(dest, buffer->data + index, length);
}
