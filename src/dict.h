#ifndef KAVY_DICT
#define KAVY_DICT

#include "main.h"

typedef struct node_s {
	char *key;
	size_t key_length;
	char *value;
	size_t value_length;
	struct node_s *next;
} DictNode;

typedef struct {
	DictNode **table;
	size_t buckets;
} Dict;

void dict_init(Dict *, size_t);
void dict_set(Dict *, char *, size_t, char *, size_t);
int dict_get(Dict *, char *, size_t, char **, size_t *);

#endif
