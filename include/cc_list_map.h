#ifndef __CC_LIST_MAP_H
#define __CC_LIST_MAP_H

#ifdef NO_MALLOC
#error "You can NOT use `cc_list_map` without support for the `malloc` function."
#endif

#include "cc_common.h"
#include "cc_iter.h"
#include "cc_list.h"
#include "cc_map.h"

struct cc_list_map {
	struct cc_map_i *interface;
	struct cc_list *data;
	cc_cmp_fn_t cmp;
};

int cc_list_map_new(struct cc_list_map **self, cc_cmp_fn_t cmp);
int cc_list_map_delete(struct cc_list_map *self);

int cc_list_map_get(struct cc_list_map *self, void *key, void **result);
int cc_list_map_set(struct cc_list_map *self, void *key, void *value, void **old_value);
int cc_list_map_set_new(struct cc_list_map *self, void *key, void *value);
int cc_list_map_del(struct cc_list_map *self, void *key, void **result);

int cc_list_map_print(struct cc_list_map *self, char *end_string);

struct cc_list_map_iter {
	struct cc_iter_i *iterator;
	struct cc_list_iter inner_iter;
};

int cc_list_map_iter_init(struct cc_list_map_iter *self, struct cc_list_map *map);
int cc_list_map_iter_next(struct cc_list_map_iter *self, void **item, size_t *index);

#endif
