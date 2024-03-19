#include "cc_list_map.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

int cc_list_map_get_item(struct cc_list_map *self, void *key, struct cc_map_item **result) {
	struct cc_list_iter iter;
	struct cc_map_item **tmp;

	*result = NULL;

	assert(cc_list_iter_init(&iter, self->data, 0));
	while (cc_iter_next(&iter, &tmp, NULL)) {
		if (self->cmp(key, (*tmp)->key) == 0) {
			*result = *tmp;
			return 1;
		}
	}

	return 0;
}

int cc_list_map_get(struct cc_list_map *self, void *key, void **result) {
	struct cc_map_item *tmp;

	*result = NULL;

	if (!cc_list_map_get_item(self, key, &tmp))
		return 0;

	*result = tmp->value;
	return 1;
}

int cc_list_map_set(struct cc_list_map *self, void *key, void *value) {
	struct cc_map_item *tmp;

	if (cc_list_map_get_item(self, key, &tmp)) {
		tmp->value = value;
		return 1;
	}

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		return 0;

	tmp->key = key;
	tmp->value = value;

	return cc_list_insert(self->data, 0, tmp);
}

void cc_list_map_print(struct cc_list_map *self, char *end_string) {
	struct cc_list_iter iter;
	struct cc_map_item **tmp;

	cc_list_iter_init(&iter, self->data, 0);
	while (cc_iter_next(&iter, &tmp, NULL))
		cc_debug_print("{%zu -> %zu} ", (*tmp)->key, (*tmp)->value);

	cc_debug_print("%s", end_string);
}

static const struct cc_map_i map_interface = {
	.get = (cc_map_get_fn)cc_list_map_get,
	.set = (cc_map_set_fn)cc_list_map_set,
};

struct cc_list_map *cc_list_map_new(cc_cmp_fn cmp) {
	struct cc_list_map *self;

	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	self->data = cc_list_new();
	if (self->data == NULL)
		return NULL;

	self->cmp = CC_WITH_DEFAULT(cmp, cc_default_cmp_fn);

	self->interface = (struct cc_map_i *)&map_interface;
	return self;
}

void cc_list_map_delete(struct cc_list_map *self) {
	struct cc_list_iter iter;
	struct cc_map_item **tmp;

	cc_list_iter_init(&iter, self->data, 0);
	while (cc_iter_next(&iter, &tmp, NULL))
		free(*tmp);

	cc_list_delete(self->data);
	free(self);
}
