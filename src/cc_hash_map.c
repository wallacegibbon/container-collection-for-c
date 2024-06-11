#include "cc_hash_map.h"
#include <stdio.h>
#include <stdlib.h>

/// Get the slot (whose type is `struct cc_list_map **`) by key.
static inline int get_list_map_ref(struct cc_hash_map *self, void *key, struct cc_list_map ***result)
{
	size_t hash_tmp;
	hash_tmp = self->calc_hash(key) % self->bucket_size;
	return cc_array_get_ref(self->data, hash_tmp, (void **)result);
}

int cc_hash_map_get(struct cc_hash_map *self, void *key, void **result)
{
	struct cc_list_map **list_map_ref;

	if (try_reset_double_p(result))
		return 1;

	if (get_list_map_ref(self, key, &list_map_ref))
		return 2;
	if (*list_map_ref == NULL)
		return 3;

	return cc_list_map_get(*list_map_ref, key, result);
}

int cc_hash_map_set_new(struct cc_hash_map *self, void *key, void *value)
{
	struct cc_list_map **list_map_ref;

	if (get_list_map_ref(self, key, &list_map_ref))
		return 1;
	if (*list_map_ref == NULL) {
		if (cc_list_map_new(list_map_ref, self->cmp))
			return 2;
	}

	return cc_list_map_set_new(*list_map_ref, key, value);
}

int cc_hash_map_set(struct cc_hash_map *self, void *key, void *value)
{
	struct cc_list_map **list_map_ref;

	if (get_list_map_ref(self, key, &list_map_ref))
		return 1;
	if (*list_map_ref == NULL) {
		if (cc_list_map_new(list_map_ref, self->cmp))
			return 2;
	}

	return cc_list_map_set(*list_map_ref, key, value);
}

int cc_hash_map_del(struct cc_hash_map *self, void *key, void **result)
{
	struct cc_list_map **list_map_ref;

	if (try_reset_double_p(result))
		return 1;

	if (get_list_map_ref(self, key, &list_map_ref))
		return 2;
	if (*list_map_ref == NULL)
		return 3;

	return cc_list_map_del(*list_map_ref, key, result);
}

int cc_hash_map_print_slot(struct cc_list_map *slot, int index)
{
	cc_debug_print("[% 9d] ", index);
	if (slot != NULL)
		return cc_list_map_print(slot, "\n");
	else
		return cc_debug_print("\n");
}

int cc_hash_map_print(struct cc_hash_map *self, char *end_string)
{
	struct cc_array_iter iter;
	struct cc_list_map **item;
	int i = 0;

	if (cc_array_iter_init(&iter, self->data))
		return 1;
	while (!cc_iter_next(&iter, &item, NULL))
		cc_hash_map_print_slot(*item, i++);

	cc_debug_print("%s", end_string);
	return 0;
}

static struct cc_map_i map_interface = {
	.get = (cc_map_get_fn_t)cc_hash_map_get,
	.set = (cc_map_set_fn_t)cc_hash_map_set,
	.set_new = (cc_map_set_fn_t)cc_hash_map_set_new,
	.del = (cc_map_del_fn_t)cc_hash_map_del,
};

int cc_hash_map_new(struct cc_hash_map **self, size_t bucket_size, cc_cmp_fn_t cmp, cc_hash_fn_t calc_hash)
{
	struct cc_hash_map *tmp;
	struct cc_list_map **item;
	struct cc_array_iter iter;
	int code = 0;

	if (bucket_size == 0) {
		code = 1;
		goto fail1;
	}

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL) {
		code = 2;
		goto fail1;
	}

	tmp->interface = &map_interface;

	code = cc_array_new(&tmp->data, bucket_size, sizeof(struct cc_list_map *));
	if (code)
		goto fail2;

	tmp->bucket_size = bucket_size;

	/// The elements of the array should be initialized as NULLs.
	code = cc_array_iter_init(&iter, tmp->data);
	if (code)
		goto fail2;

	while (!cc_iter_next(&iter, &item, NULL))
		*item = NULL;

	tmp->cmp = CC_WITH_DEFAULT(cmp, cc_default_cmp_fn);
	tmp->calc_hash = CC_WITH_DEFAULT(calc_hash, cc_default_hash_fn);

	*self = tmp;
	return 0;

fail2:
	free(tmp);
fail1:
	return code;
}

int cc_hash_map_delete(struct cc_hash_map *self)
{
	struct cc_array_iter iter;
	struct cc_list_map **item;

	if (cc_array_iter_init(&iter, self->data))
		return 1;
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

static struct cc_iter_i iterator_interface = {
	.next = (cc_iter_next_fn_t)cc_hash_map_iter_next,
};

/// Initialize the self->inner_list_map_iter
static int cc_hash_map_iter_step(struct cc_hash_map_iter *self)
{
	struct cc_list_map **cursor;

	while (1) {
		if (cc_iter_next(&self->inner_array_iter, (void **)&cursor, NULL))
			return -1;
		if (*cursor != NULL)
			break;
	}

	return cc_list_map_iter_init(&self->inner_list_map_iter, *cursor);
}

int cc_hash_map_iter_next(struct cc_hash_map_iter *self, void **item, size_t *index)
{
	if (try_reset_double_p(item))
		return 1;
	if (self->is_empty)
		return 2;

	if (!cc_iter_next(&self->inner_list_map_iter, item, NULL))
		goto success;

	if (cc_hash_map_iter_step(self))
		return 3;
	if (cc_iter_next(&self->inner_list_map_iter, item, NULL))
		return 4;

success:
	if (index != NULL)
		*index = self->count;

	self->count++;
	return 0;
}

int cc_hash_map_iter_init(struct cc_hash_map_iter *self, struct cc_hash_map *map)
{
	int tmp;

	if (map == NULL)
		return 1;
	if (cc_array_iter_init(&self->inner_array_iter, map->data))
		return 2;

	self->iterator = &iterator_interface;
	self->count = 0;
	self->is_empty = 0;

	/// `hash_map`'s `init` will call `step`, which is dangerous. Be careful with code here.
	tmp = cc_hash_map_iter_step(self);
	if (tmp == 0)
		return 0;

	/// unhandled error when calling `_step`
	if (tmp != -1)
		return 3;

	/// set the flag so `_next` knows when it should return directly.
	self->is_empty = 1;
	return 0;
}
