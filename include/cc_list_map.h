#ifndef __CC_LIST_MAP_H
#define __CC_LIST_MAP_H

#include "cc_common.h"
#include "cc_iter.h"
#include "cc_list.h"
#include "cc_map.h"

typedef struct cc_ListMap cc_ListMap;

struct cc_ListMap {
	cc_MapI *interface;
	cc_List *data;
	cc_ListCursor cursor;
	cc_CmpFn cmp;
};

int	cc_list_map_new(cc_ListMap **self, cc_CmpFn cmp);
int	cc_list_map_delete(cc_ListMap *self);
int	cc_list_map_get(cc_ListMap *self, void *key, void **result);
int	cc_list_map_set(cc_ListMap *self, void *key, void *value, void **old_value);
int	cc_list_map_set_new(cc_ListMap *self, void *key, void *value);
int	cc_list_map_del(cc_ListMap *self, void *key, cc_MapItem **result);
int	cc_list_map_print(cc_ListMap *self, char *end_string);

typedef struct cc_ListMapIter cc_ListMapIter;

struct cc_ListMapIter {
	cc_IterI *iterator;
	cc_ListIter inner_iter;
};

int	cc_list_map_iter_init(cc_ListMapIter *self, cc_ListMap *map);
int	cc_list_map_iter_next(cc_ListMapIter *self, void **item, size_t *index);

#endif
