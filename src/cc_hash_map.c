#include "cc_hash_map.h"
#include "cc_array.h"
#include "cc_iter.h"
#include "cc_map.h"
#include <stdlib.h>

static const struct cc_map_i map_interface = {
	.get = (cc_map_get_fn)cc_hash_map_get,
	.set = (cc_map_set_fn)cc_hash_map_set,
};

struct cc_hash_map *cc_hash_map_new() {
	struct cc_hash_map *self;
	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	self->data = cc_array_new(8, 8 /*placeholder*/);
	if (self->data == NULL)
		return NULL;

	self->interface = (struct cc_map_i *)&map_interface;

	return self;
}

void cc_hash_map_delete(struct cc_hash_map *self, cc_cleanup_fn fn) {
	struct cc_array_iter iter;
	uint8_t tmp[8 /*placeholder*/];

	cc_array_iter_init(&iter, self->data);
	while (cc_iter_next(&iter, tmp))
		fn(tmp /*placeholder, fetch value from item pair */);

	cc_array_delete(self->data, free);
	free(self);
}

int cc_hash_map_get(struct cc_hash_map *self, void *key, void *value) {
	return 1;
}

int cc_hash_map_set(struct cc_hash_map *self, void *key, void *value) {
	return 1;
}
