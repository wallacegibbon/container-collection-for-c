#include "cc_list_map.h"
#include <stdio.h>
#include <stdlib.h>

int cc_list_map_get_item(struct cc_list_map *self, void *key, struct cc_map_item **result, size_t *index) {
	struct cc_list_iter iter;
	struct cc_map_item **tmp;

	if (try_reset_double_p(result))
		return 1;
	if (cc_list_iter_init(&iter, self->data, 0))
		return 2;
	while (!cc_iter_next(&iter, &tmp, index)) {
		if (self->cmp(key, (*tmp)->key) == 0) {
			*result = *tmp;
			return 0;
		}
	}

	return 3;
}

int cc_list_map_get(struct cc_list_map *self, void *key, void **result) {
	struct cc_map_item *tmp;

	if (try_reset_double_p(result))
		return 1;
	if (cc_list_map_get_item(self, key, &tmp, NULL))
		return 2;

	*result = tmp->value;
	return 0;
}

int cc_list_map_set(struct cc_list_map *self, void *key, void *value) {
	struct cc_map_item *tmp;

	if (!cc_list_map_get_item(self, key, &tmp, NULL)) {
		tmp->value = value;
		return 0;
	}

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		return 1;

	tmp->key = key;
	tmp->value = value;
	return cc_list_insert(self->data, 0, tmp);
}

int cc_list_map_del(struct cc_list_map *self, void *key, void **result) {
	struct cc_map_item *tmp;
	size_t index;

	if (try_reset_double_p(result))
		return 1;
	if (cc_list_map_get_item(self, key, &tmp, &index))
		return 2;
	if (cc_list_remove(self->data, index, (void **)&tmp))
		return 3;

	*result = tmp->value;
	free(tmp);
	return 0;
}

int cc_list_map_print(struct cc_list_map *self, char *end_string) {
	struct cc_list_map_iter iter;
	struct cc_map_item *tmp;
	size_t index;

	if (cc_list_map_iter_init(&iter, self))
		return 1;
	while (!cc_iter_next(&iter, &tmp, &index))
		cc_debug_print("(%d){%zu -> %zu} ", index, tmp->key, tmp->value);

	cc_debug_print("%s", end_string);
	return 0;
}

static const struct cc_map_i map_interface = {
	.get = (cc_map_get_fn)cc_list_map_get,
	.set = (cc_map_set_fn)cc_list_map_set,
	.del = (cc_map_del_fn)cc_list_map_del,
};

struct cc_list_map *cc_list_map_new(cc_cmp_fn cmp) {
	struct cc_list_map *self;

	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	self->interface = (struct cc_map_i *)&map_interface;
	self->data = cc_list_new();
	if (self->data == NULL)
		return NULL;

	self->cmp = CC_WITH_DEFAULT(cmp, cc_default_cmp_fn);

	return self;
}

int cc_list_map_delete(struct cc_list_map *self) {
	struct cc_list_iter iter;
	struct cc_map_item **tmp;

	if (cc_list_iter_init(&iter, self->data, 0))
		return 1;
	while (!cc_iter_next(&iter, &tmp, NULL))
		free(*tmp);

	cc_list_delete(self->data);
	free(self);
	return 0;
}

static const struct cc_iter_i iterator_interface = {
	.next = (cc_iter_next_fn)cc_list_map_iter_next,
};

int cc_list_map_iter_init(struct cc_list_map_iter *self, struct cc_list_map *map) {
	if (map == NULL)
		return 1;

	self->iterator = (struct cc_iter_i *)&iterator_interface;
	return cc_list_iter_init(&self->inner_iter, map->data, 0);
}

int cc_list_map_iter_next(struct cc_list_map_iter *self, void **item, size_t *index) {
	struct cc_map_item **tmp;

	if (try_reset_double_p(item))
		return 1;
	if (cc_list_iter_next(&self->inner_iter, (void **)&tmp, index))
		return 2;

	*item = *tmp;
	return 0;
}
