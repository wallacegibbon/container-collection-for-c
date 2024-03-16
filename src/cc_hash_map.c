#include "cc_hash_map.h"
#include "cc_array.h"
#include "cc_common.h"
#include "cc_iter.h"
#include "cc_list_map.h"
#include "cc_map.h"
#include <assert.h>
#include <stdlib.h>

static inline size_t calc_hash(struct cc_hash_map *self, void *key) {
	return self->calc_hash(key) % self->bucket_size;
}

int cc_hash_map_get_item(struct cc_hash_map *self, void *key, struct cc_map_item **result) {
	struct cc_map_item tmp;
	struct cc_list_map *list_map_tmp;
	size_t hash_tmp;

	*result = NULL;

	hash_tmp = calc_hash(self, key);
	assert(cc_array_get(self->data, hash_tmp, &list_map_tmp));

	if (list_map_tmp != NULL)
		return cc_list_map_get_item(list_map_tmp, key, result);
	else
		return 0;
}

int cc_hash_map_get(struct cc_hash_map *self, void *key, void **result) {
	struct cc_map_item *tmp;
	if (!cc_hash_map_get_item(self, key, &tmp))
		return 0;

	*result = tmp->value;
	return 1;
}

int cc_hash_map_set(struct cc_hash_map *self, void *key, void *value) {
	struct cc_list_map *list_map_tmp;
	struct cc_map_item *tmp;
	size_t hash_tmp;

	if (cc_hash_map_get_item(self, key, &tmp)) {
		tmp->value = value;
		return 1;
	}

	hash_tmp = calc_hash(self, key);
	assert(cc_array_get(self->data, hash_tmp, &list_map_tmp));

	if (list_map_tmp == NULL) {
		list_map_tmp = cc_list_map_new(self->cmp);
		cc_array_set(self->data, hash_tmp, list_map_tmp);
	}

	return cc_list_map_set(list_map_tmp, key, value);
}

static const struct cc_map_i map_interface = {
	.get = (cc_map_get_fn)cc_hash_map_get,
	.set = (cc_map_set_fn)cc_hash_map_set,
};

struct cc_hash_map *cc_hash_map_new(size_t bucket_size, cc_cmp_fn cmp, cc_hash_fn calc_hash) {
	struct cc_hash_map *self;
	struct cc_array_iter iter;
	struct cc_list_map **tmp;

	if (bucket_size == 0)
		return NULL;

	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	self->data = cc_array_new(bucket_size, sizeof(struct cc_list_map *));
	if (self->data == NULL)
		return NULL;

	self->bucket_size = bucket_size;

	/// The elements of the array should be initialized as NULLs.
	cc_array_iter_init(&iter, self->data);
	while (cc_iter_next(&iter, &tmp))
		*tmp = NULL;

	self->cmp = CC_WITH_DEFAULT(cmp, cc_default_cmp_fn);
	self->calc_hash = CC_WITH_DEFAULT(calc_hash, cc_default_hash_fn);

	self->interface = (struct cc_map_i *)&map_interface;

	return self;
}

void cc_hash_map_delete(struct cc_hash_map *self) {
	struct cc_array_iter iter;
	struct cc_list_map *tmp;

	cc_array_iter_init(&iter, self->data);
	while (cc_iter_next(&iter, &tmp))
		cc_list_map_delete(tmp);

	cc_array_delete(self->data, NULL);
	free(self);
}
