#ifndef KAVY_BUFFER

#include "main.h"

typedef struct {
	char *data;
	size_t length;
	size_t container;
} Buffer;

void buffer_init(Buffer *);
void buffer_free(Buffer *);
void buffer_ltrim(Buffer *, size_t);
void buffer_append(Buffer *, char *, size_t);
void buffer_slice(Buffer *, size_t, void *, size_t);

#endif
