#ifndef __CC_HASH_MAP_H
#define __CC_HASH_MAP_H

#include "cc_array.h"
#include "cc_common.h"
#include "cc_iter.h"
#include "cc_list_map.h"
#include "cc_map.h"
#include <stddef.h>

typedef struct cc_HashMap cc_HashMap;

struct cc_HashMap {
	cc_MapI *interface;
	/* `data` is an array whose elements are of type `cc_ListMap *` */
	cc_Array *data;
	size_t bucket_size;
	cc_CmpFn cmp;
	cc_HashFn calc_hash;
};

int	cc_hash_map_new(cc_HashMap **self, size_t bucket_size, cc_CmpFn cmp,
		cc_HashFn calc_hash);
int	cc_hash_map_delete(cc_HashMap *self);
int	cc_hash_map_get(cc_HashMap *self, void *key, void **result);
int	cc_hash_map_set_new(cc_HashMap *self, void *key, void *value);
int	cc_hash_map_set(cc_HashMap *self, void *key, void *value,
		void **old_value);
int	cc_hash_map_del(cc_HashMap *self, void *key, cc_MapItem **result);
int	cc_hash_map_print(cc_HashMap *self, char *end_string);

typedef struct cc_HashMapIter cc_HashMapIter;

struct cc_HashMapIter {
	cc_IterI *iterator;
	cc_ArrayIter inner_array_iter;
	cc_ListMapIter inner_list_map_iter;
	size_t count;
	unsigned char is_empty;
};

int	cc_hash_map_iter_init(cc_HashMapIter *self, cc_HashMap *map);
int	cc_hash_map_iter_next(cc_HashMapIter *self, void **item, size_t *index);

#endif
