#ifndef __CC_LIST_MAP_H
#define __CC_LIST_MAP_H

#ifdef NO_MALLOC
#error "You can NOT use list map without support for the `malloc` function."
#endif

#include "cc_common.h"
#include "cc_list.h"
#include "cc_map.h"
#include <stddef.h>

struct cc_list_map {
	struct cc_map_i *interface;
	struct cc_list *data;
	cc_cmp_fn cmp;
	size_t item_count;
};

struct cc_list_map *cc_list_map_new(cc_cmp_fn cmp);
void cc_list_map_delete(struct cc_list_map *self);

int cc_list_map_get_item(struct cc_list_map *self, void *key, struct cc_map_item **result);
int cc_list_map_get(struct cc_list_map *self, void *key, void **result);
int cc_list_map_set(struct cc_list_map *self, void *key, void *value);

void cc_list_map_print(struct cc_list_map *self, char *end_string);

#endif
