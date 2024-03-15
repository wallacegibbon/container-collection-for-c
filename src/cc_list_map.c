#include "cc_list_map.h"
#include "cc_iter.h"
#include "cc_list.h"
#include "cc_map.h"
#include <stdlib.h>

static const struct cc_map_i map_interface = {
	.get = (cc_map_get_fn)cc_list_map_get,
	.set = (cc_map_set_fn)cc_list_map_set,
};

struct cc_list_map *cc_list_map_new() {
	struct cc_list_map *self;
	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	self->data = cc_list_new();
	if (self->data == NULL)
		return NULL;

	self->interface = (struct cc_map_i *)&map_interface;

	return self;
}

void cc_list_map_delete(struct cc_list_map *self, cc_cleanup_fn fn) {
	struct cc_list_iter iter;
	uint8_t tmp[8 /*placeholder*/];

	cc_list_iter_init(&iter, self->data, 0);
	while (cc_iter_next(&iter, tmp))
		fn(tmp /*placeholder, fetch value from item pair */);

	cc_list_delete(self->data, NULL);
	free(self);
}

int cc_list_map_get(struct cc_list_map *self, void *key, void *value) {
	return 1;
}

int cc_list_map_set(struct cc_list_map *self, void *key, void *value) {
	return 1;
}
