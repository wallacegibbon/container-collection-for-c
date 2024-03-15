#ifndef __CC_HASH_MAP_H
#define __CC_HASH_MAP_H

#ifdef NO_MALLOC
#error "You can NOT use hash map without support for the `malloc` function."
#endif

#include "cc_common.h"
#include "cc_array.h"
#include "cc_map.h"
#include <stddef.h>

struct cc_hash_map {
	struct cc_map_i *interface;
	/// An array of `struct cc_list_map *`
	struct cc_array *data;
	size_t item_count;
};

struct cc_hash_map *cc_hash_map_new();
void cc_hash_map_delete(struct cc_hash_map *self, cc_cleanup_fn fn);

int cc_hash_map_get(struct cc_hash_map *self, void *key, void *value);
int cc_hash_map_set(struct cc_hash_map *self, void *key, void *value);

#endif
