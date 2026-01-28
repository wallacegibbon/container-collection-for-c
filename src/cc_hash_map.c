#include "cc_hash_map.h"
#include "cc_array.h"
#include <stdio.h>
#include <stdlib.h>

struct cc_map_i cc_hash_map_interface = {
	.get = (cc_map_get_fn_t)cc_hash_map_get,
	.set = (cc_map_set_fn_t)cc_hash_map_set,
	.set_new = (cc_map_set_new_fn_t)cc_hash_map_set_new,
	.del = (cc_map_del_fn_t)cc_hash_map_del,
};

struct cc_iter_i cc_hash_map_iter_interface = {
	.next = (cc_iter_next_fn_t)cc_hash_map_iter_next,
};

/// Get the slot (whose type is `struct cc_list_map **`) by key.
static inline void get_list_map_ref(struct cc_hash_map *self, void *key, struct cc_list_map ***result) {
	size_t hash_tmp = self->calc_hash(key) % self->bucket_size;
	cc_array_get_ref_unsafe(self->data, hash_tmp, (void **)result);
}

int cc_hash_map_get(struct cc_hash_map *self, void *key, void **result) {
	if (try_reset_double_p(result))
		return 1;

	struct cc_list_map **list_map_ref;
	get_list_map_ref(self, key, &list_map_ref);
	if (*list_map_ref == NULL)
		return CC_MAP_KEY_NOT_FOUND;

	return cc_list_map_get(*list_map_ref, key, result);
}

int cc_hash_map_set_new(struct cc_hash_map *self, void *key, void *value) {
	struct cc_list_map **list_map_ref;
	get_list_map_ref(self, key, &list_map_ref);
	if (*list_map_ref == NULL) {
		if (cc_list_map_new(list_map_ref, self->cmp))
			return 2;
	}

	return cc_list_map_set_new(*list_map_ref, key, value);
}

int cc_hash_map_set(struct cc_hash_map *self, void *key, void *value, void **old_value) {
	struct cc_list_map **list_map_ref;
	get_list_map_ref(self, key, &list_map_ref);
	if (*list_map_ref == NULL) {
		if (cc_list_map_new(list_map_ref, self->cmp))
			return 2;
	}

	return cc_list_map_set(*list_map_ref, key, value, old_value);
}

int cc_hash_map_del(struct cc_hash_map *self, void *key, struct cc_map_item **result) {
	if (try_reset_double_p(result))
		return 1;

	struct cc_list_map **list_map_ref;
	get_list_map_ref(self, key, &list_map_ref);
	if (*list_map_ref == NULL)
		return CC_MAP_KEY_NOT_FOUND;

	return cc_list_map_del(*list_map_ref, key, result);
}

int cc_hash_map_print_slot(struct cc_list_map *slot, int index) {
	cc_debug_print("[% 9d] ", index);
	if (slot != NULL)
		return cc_list_map_print(slot, "\n");

	cc_debug_print("\n");
	return 0;
}

int cc_hash_map_print(struct cc_hash_map *self, char *end_string) {
	struct cc_array_iter iter;
	if (cc_array_iter_init(&iter, self->data))
		return 1;

	struct cc_list_map **item;
	for (int i = 0; !cc_iter_next(&iter, &item, NULL); i++)
		cc_hash_map_print_slot(*item, i);

	cc_debug_print("%s", end_string);
	return 0;
}

int cc_hash_map_new(struct cc_hash_map **self, size_t bucket_size, cc_cmp_fn_t cmp, cc_hash_fn_t calc_hash) {
	if (bucket_size == 0)
		goto fail1;

	struct cc_hash_map *tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;

	tmp->interface = &cc_hash_map_interface;

	if (cc_array_new(&tmp->data, bucket_size, sizeof(struct cc_list_map *)))
		goto fail2;

	tmp->bucket_size = bucket_size;

	/// The elements of the array should be initialized as NULLs.
	struct cc_array_iter iter;
	if (cc_array_iter_init(&iter, tmp->data))
		goto fail3;

	struct cc_list_map **item;
	while (!cc_iter_next(&iter, &item, NULL))
		*item = NULL;

	tmp->cmp = CC_WITH_DEFAULT(cmp, cc_default_cmp_fn);
	tmp->calc_hash = CC_WITH_DEFAULT(calc_hash, cc_default_hash_fn);

	*self = tmp;
	return 0;

fail3:
	cc_array_delete(tmp->data);
fail2:
	free(tmp);
fail1:
	return 1;
}

int cc_hash_map_delete(struct cc_hash_map *self) {
	struct cc_array_iter iter;
	if (cc_array_iter_init(&iter, self->data))
		return 1;

	struct cc_list_map **item;
	while (!cc_iter_next(&iter, &item, NULL)) {
		if (*item != NULL) {
			if (cc_list_map_delete(*item))
				return 2;
		}
	}

	if (cc_array_delete(self->data))
		return 3;

	free(self);
	return 0;
}

/// Initialize the self->inner_list_map_iter
static int cc_hash_map_iter_step(struct cc_hash_map_iter *self) {
	struct cc_list_map **cursor;
	while (1) {
		if (cc_iter_next(&self->inner_array_iter, (void **)&cursor, NULL))
			return -1;
		if (*cursor != NULL)
			break;
	}

	return cc_list_map_iter_init(&self->inner_list_map_iter, *cursor);
}

int cc_hash_map_iter_next(struct cc_hash_map_iter *self, struct cc_map_item **item, size_t *index) {
	if (try_reset_double_p(item))
		return 1;
	if (self->is_empty)
		return 2;

	while (1) {
		/// Try to get the item directly from the current list map.
		if (!cc_iter_next(&self->inner_list_map_iter, item, NULL))
			break;
		/// If the list in current array slot is empty, move to next slot.
		if (cc_hash_map_iter_step(self))
			return 3;
	}

	if (index != NULL)
		*index = self->count;

	self->count++;
	return 0;
}

int cc_hash_map_iter_init(struct cc_hash_map_iter *self, struct cc_hash_map *map) {
	if (map == NULL)
		return 1;
	if (cc_array_iter_init(&self->inner_array_iter, map->data))
		return 2;

	self->iterator = &cc_hash_map_iter_interface;
	self->count = 0;
	self->is_empty = 0;

	/// `hash_map`'s `init` will call `step`, which is dangerous.  Be careful with code here.
	int tmp = cc_hash_map_iter_step(self);
	if (tmp == 0)
		return 0;

	/// Unhandled error when calling `_step`.
	if (tmp != -1)
		return 3;

	/// Set the flag so `_next` knows when it should return directly.
	self->is_empty = 1;
	return 0;
}
