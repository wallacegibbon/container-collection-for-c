#ifndef __CC_HASH_MAP_H
#define __CC_HASH_MAP_H

#ifdef NO_MALLOC
#error "You can NOT use hash map without support for the `malloc` function."
#endif

#include "cc_array.h"
#include "cc_common.h"
#include "cc_map.h"
#include <stddef.h>

struct cc_hash_map {
	struct cc_map_i *interface;
	struct cc_array *data; /// An array of `struct cc_list_map *`
	size_t bucket_size;
	cc_cmp_fn cmp;
	cc_hash_fn calc_hash;
};

struct cc_hash_map *cc_hash_map_new(size_t bucket_size, cc_cmp_fn cmp, cc_hash_fn calc_hash);
void cc_hash_map_delete(struct cc_hash_map *self);

int cc_hash_map_get(struct cc_hash_map *self, void *key, void **result);
int cc_hash_map_set(struct cc_hash_map *self, void *key, void *value);
int cc_hash_map_del(struct cc_hash_map *self, void *key, void **result);

void cc_hash_map_print(struct cc_hash_map *self, char *end_string);

#endif
