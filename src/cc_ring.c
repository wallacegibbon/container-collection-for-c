#include "cc_ring.h"
#include <stdlib.h>

static inline size_t _next_index(struct cc_ring *self, size_t index) {
	size_t next_index = index + 1;
	if (cc_array_is_valid_index(self->data, next_index))
		return next_index;
	else
		return 0;
}

size_t cc_ring_space(struct cc_ring *self) {
	if (self->write_index >= self->read_index)
		return self->data->elem_nums - (self->write_index - self->read_index) - 1;
	else
		// return self->data->elem_nums - (self->data->elem_nums + self->write_index - self->read_index) - 1;
		return self->read_index - self->write_index - 1;
}

int cc_ring_append(struct cc_ring *self, void *item) {
	size_t write_index_next;

	write_index_next = _next_index(self, self->write_index);
	if (write_index_next == self->read_index)
		return 1;

	cc_array_set(self->data, self->write_index, item);
	self->write_index = write_index_next;

	return 0;
}

int cc_ring_shift(struct cc_ring *self, void *item) {
	if (self->read_index == self->write_index)
		return 1;

	cc_array_get(self->data, self->read_index, item);
	self->read_index = _next_index(self, self->read_index);

	return 0;
}

int cc_ring_init(struct cc_ring *self, struct cc_array *data) {
	self->data = data;
	self->read_index = 0;
	self->write_index = 0;
	return 0;
}

#ifndef NO_MALLOC

struct cc_ring *cc_ring_new(size_t elem_nums, size_t elem_size) {
	struct cc_ring *self;
	struct cc_array *data;

	self = malloc(sizeof(*self));
	if (self == NULL)
		goto fail1;

	/// 1 element will be wasted, so pass `elem_nums + 1` to `cc_array_new`.
	data = cc_array_new(elem_nums + 1, elem_size);
	if (data == NULL)
		goto fail2;

	if (cc_ring_init(self, data))
		goto fail2;

	return self;

fail2:
	free(self);
fail1:
	return NULL;
}

int cc_ring_delete(struct cc_ring *self) {
	if (cc_array_delete(self->data))
		return 1;
	free(self);
	return 0;
}

#endif
