#ifndef __CC_HASH_MAP_H
#define __CC_HASH_MAP_H

#ifdef NO_MALLOC
#error "You can NOT use `cc_hash_map` without support for the `malloc` function."
#endif

#include "cc_array.h"
#include "cc_common.h"
#include "cc_iter.h"
#include "cc_list_map.h"
#include "cc_map.h"
#include <stddef.h>

struct cc_hash_map {
	struct cc_map_i *interface;
	struct cc_array *data; /// An array of `struct cc_list_map *`
	size_t bucket_size;
	cc_cmp_fn_t cmp;
	cc_hash_fn_t calc_hash;
};

int cc_hash_map_new(struct cc_hash_map **self, size_t bucket_size, cc_cmp_fn_t cmp, cc_hash_fn_t calc_hash);
int cc_hash_map_delete(struct cc_hash_map *self);

int cc_hash_map_get(struct cc_hash_map *self, void *key, void **result);
int cc_hash_map_set(struct cc_hash_map *self, void *key, void *value, void **old_value);
int cc_hash_map_del(struct cc_hash_map *self, void *key, void **result);

int cc_hash_map_print(struct cc_hash_map *self, char *end_string);

struct cc_hash_map_iter {
	struct cc_iter_i *iterator;
	struct cc_array_iter inner_array_iter;
	struct cc_list_map_iter inner_list_map_iter;
	size_t count;
	unsigned char is_empty;
};

int cc_hash_map_iter_init(struct cc_hash_map_iter *self, struct cc_hash_map *map);
int cc_hash_map_iter_next(struct cc_hash_map_iter *self, void **item, size_t *index);

#endif
