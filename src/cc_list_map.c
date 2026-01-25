#include "cc_list_map.h"
#include "cc_list.h"
#include <stdio.h>
#include <stdlib.h>

struct cc_map_i cc_list_map_interface = {
	.get = (cc_map_get_fn_t)cc_list_map_get,
	.set = (cc_map_set_fn_t)cc_list_map_set,
	.set_new = (cc_map_set_new_fn_t)cc_list_map_set_new,
	.del = (cc_map_del_fn_t)cc_list_map_del,
};

struct cc_iter_i cc_list_map_iter_interface = {
	.next = (cc_iter_next_fn_t)cc_list_map_iter_next,
};

static inline int cc_list_map_get_current(struct cc_list_map *self, struct cc_map_item **item) {
	return cc_list_cursor_get(&self->cursor, 0, 1, (void **)item);
}

/// The caller should make sure that `result` is not NULL.
static int cc_list_map_move_to_item(struct cc_list_map *self, void *key) {
	struct cc_map_item *tmp;

	cc_list_cursor_reset(&self->cursor);

	for (; !cc_list_map_get_current(self, &tmp); cc_list_cursor_move(&self->cursor, 1)) {
		if (self->cmp(key, tmp->key) == 0)
			return 0;
	}

	return CC_MAP_KEY_NOT_FOUND;
}

int cc_list_map_get(struct cc_list_map *self, void *key, void **result) {
	struct cc_map_item *item;
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

int cc_list_map_insert_new(struct cc_list_map *self, void *key, void *value) {
	struct cc_map_item *item;

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

int cc_list_map_set_new(struct cc_list_map *self, void *key, void *value) {
	struct cc_map_item *item;

	if (!cc_list_map_move_to_item(self, key))
		return CC_MAP_KEY_ALREADY_EXIST;
	if (cc_list_map_insert_new(self, key, value))
		return 2;

	return 0;
}

/// CAUTION: When `old_value` is NULL, the old value of `key` will be ignored, which may cause memory leak.
int cc_list_map_set(struct cc_list_map *self, void *key, void *value, void **old_value) {
	struct cc_map_item *item;

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

int cc_list_map_del(struct cc_list_map *self, void *key, struct cc_map_item **result) {
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

int cc_list_map_print(struct cc_list_map *self, char *end_string) {
	struct cc_list_map_iter iter;
	struct cc_map_item *item;
	size_t index;

	if (cc_list_map_iter_init(&iter, self))
		return 1;
	while (!cc_iter_next(&iter, &item, &index))
		cc_debug_print("(%d){%zu -> %zu} ", index, item->key, item->value);

	cc_debug_print("%s", end_string);
	return 0;
}

int cc_list_map_new(struct cc_list_map **self, cc_cmp_fn_t cmp) {
	struct cc_list_map *tmp;

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

int cc_list_map_delete(struct cc_list_map *self) {
	struct cc_list_iter iter;
	struct cc_map_item **item;

	if (cc_list_iter_init(&iter, self->data, 0))
		return 1;
	while (!cc_iter_next(&iter, &item, NULL))
		free(*item);

	if (cc_list_delete(self->data))
		return 2;

	free(self);
	return 0;
}

int cc_list_map_iter_next(struct cc_list_map_iter *self, struct cc_map_item **item, size_t *index) {
	struct cc_map_item **tmp;

	if (try_reset_double_p(item))
		return 1;
	if (cc_list_iter_next(&self->inner_iter, (void **)&tmp, index))
		return 2;

	*item = *tmp;
	return 0;
}

int cc_list_map_iter_init(struct cc_list_map_iter *self, struct cc_list_map *map) {
	if (map == NULL)
		return 1;

	self->iterator = &cc_list_map_iter_interface;
	return cc_list_iter_init(&self->inner_iter, map->data, 0);
}
