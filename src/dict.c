#include "dict.h"

void dict_init(Dict *dict, size_t buckets) {
	dict->table = calloc(dict->buckets = buckets, sizeof(DictNode *));
}

size_t dict_hash(Dict *dict, char *data, size_t data_length) {
	size_t h = 5381;

	for(size_t i = 0; i < data_length; i++) {
		h = h * 33 ^ (unsigned char) data[i];
	}

	return h % dict->buckets;
}

void dict_set(Dict *dict, char *key, size_t key_length, char *value, size_t value_length) {
	const size_t hash = dict_hash(dict, key, key_length);

	DictNode *new = calloc(1, sizeof(DictNode));

	if(new == NULL)
		error("calloc");

	if((new->key = malloc(key_length)) == NULL)
		error("malloc");

	memcpy(new->key, key, new->key_length = key_length);

	if((new->value = malloc(value_length)) == NULL)
		error("malloc");

	memcpy(new->value, value, new->value_length = value_length);

	DictNode **node = &dict->table[hash];

	while(*node != NULL) {
		node = &(*node)->next;
	}

	*node = new;
}

int dict_get(Dict *dict, char *key, size_t key_length, char **value, size_t *value_length) {
	const size_t hash = dict_hash(dict, key, key_length);

	DictNode *node = dict->table[hash];

	while(node != NULL) {
		if(node->key_length == key_length && memcmp(node->key, key, key_length) == 0) {
			*value = node->value;
			*value_length = node->value_length;

			return 1;
		}

		node = node->next;
	}

	return 0;
}
