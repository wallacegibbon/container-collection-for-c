#include "cc_list_map.h"
#include "cc_list.h"
#include <stdio.h>
#include <stdlib.h>

cc_MapI cc_list_map_interface = {
	.get = (cc_MapGetFn)cc_list_map_get,
	.set = (cc_MapSetFn)cc_list_map_set,
	.set_new = (cc_MapSetNewFn)cc_list_map_set_new,
	.del = (cc_MapDelFn)cc_list_map_del,
};

cc_IterI cc_list_map_iter_interface = {
	.next = (cc_IterNextFn)cc_list_map_iter_next,
};

static inline int cc_list_map_get_current(cc_ListMap *self, cc_MapItem **item)
{
	return cc_list_cursor_get(&self->cursor, 0, 1, (void **)item);
}

/* The caller should make sure that `result` is not NULL. */
static int cc_list_map_move_to_item(cc_ListMap *self, void *key)
{
	cc_MapItem *tmp;

	cc_list_cursor_reset(&self->cursor);

	for (; !cc_list_map_get_current(self, &tmp);
			cc_list_cursor_move(&self->cursor, 1)) {
		if (self->cmp(key, tmp->key) == 0)
			return 0;
	}

	return CC_MAP_KEY_NOT_FOUND;
}

int cc_list_map_get(cc_ListMap *self, void *key, void **result)
{
	cc_MapItem *item;
	int code;

	if (try_reset_double_p(result))
		return 1;

	if (cc_list_map_move_to_item(self, key))
		return CC_MAP_KEY_NOT_FOUND;
	if (cc_list_map_get_current(self, &item))
		return 1;

	*result = item->value;
	return 0;
}

int cc_list_map_insert_new(cc_ListMap *self, void *key, void *value)
{
	cc_MapItem *item;

	item = malloc(sizeof(*item));
	if (item == NULL)
		goto fail1;

	item->key = key;
	item->value = value;
	if (cc_list_insert_tail(self->data, item))
		goto fail2;

	return 0;

fail2:
	free(item);
fail1:
	return 1;
}

int cc_list_map_set_new(cc_ListMap *self, void *key, void *value)
{
	cc_MapItem *item;

	if (!cc_list_map_move_to_item(self, key))
		return CC_MAP_KEY_ALREADY_EXIST;
	if (cc_list_map_insert_new(self, key, value))
		return 2;

	return 0;
}

/* CAUTION:
 * When `old_value` is NULL, the old value of `key` will be ignored,
 * which may cause memory leak.
 */
int cc_list_map_set(cc_ListMap *self, void *key, void *value, void **old_value)
{
	cc_MapItem *item;

	if (cc_list_map_move_to_item(self, key)) {
		if (cc_list_map_insert_new(self, key, value))
			return 1;
		if (old_value != NULL)
			*old_value = NULL;
	} else {
		if (cc_list_map_get_current(self, &item))
			return 2;
		item->value = value;
		if (old_value != NULL)
			*old_value = item->value;
	}
	return 0;
}

int cc_list_map_del(cc_ListMap *self, void *key, cc_MapItem **result)
{
	if (try_reset_double_p(result))
		return 1;
	if (cc_list_map_move_to_item(self, key))
		return CC_MAP_KEY_NOT_FOUND;
	if (cc_list_map_get_current(self, result))
		return 2;
	if (cc_list_cursor_move(&self->cursor, 1))
		return 3;
	if (cc_list_cursor_remove(&self->cursor, -1, 1))
		return 4;

	return 0;
}

int cc_list_map_print(cc_ListMap *self, char *end_string)
{
	cc_ListMapIter iter;
	cc_MapItem *item;
	size_t index;

	if (cc_list_map_iter_init(&iter, self))
		return 1;
	while (!cc_iter_next(&iter, &item, &index))
		cc_debug_print("(%d){%zu -> %zu} ", index, item->key,
				item->value);

	cc_debug_print("%s", end_string);
	return 0;
}

int cc_list_map_new(cc_ListMap **self, cc_CmpFn cmp)
{
	cc_ListMap *tmp;

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;

	tmp->interface = &cc_list_map_interface;

	if (cc_list_new(&tmp->data))
		goto fail2;
	if (cc_list_cursor_init(&tmp->cursor, tmp->data, NULL))
		goto fail3;

	tmp->cmp = CC_WITH_DEFAULT(cmp, cc_default_cmp_fn);
	*self = tmp;
	return 0;

fail3:
	cc_list_delete(tmp->data);
fail2:
	free(tmp);
fail1:
	return 1;
}

int cc_list_map_delete(cc_ListMap *self)
{
	cc_ListIter iter;
	cc_MapItem **item;

	if (cc_list_iter_init(&iter, self->data, 0))
		return 1;
	while (!cc_iter_next(&iter, &item, NULL))
		free(*item);

	if (cc_list_delete(self->data))
		return 2;

	free(self);
	return 0;
}

int cc_list_map_iter_next(cc_ListMapIter *self, void **item, size_t *index)
{
	cc_MapItem **tmp_item;

	if (try_reset_double_p(item))
		return 1;
	if (cc_list_iter_next(&self->inner_iter, (void **)&tmp_item, index))
		return 2;

	*item = *tmp_item;
	return 0;
}

int cc_list_map_iter_init(cc_ListMapIter *self, cc_ListMap *map)
{
	if (map == NULL)
		return 1;

	self->iterator = &cc_list_map_iter_interface;
	return cc_list_iter_init(&self->inner_iter, map->data, 0);
}
