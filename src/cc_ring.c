#include "cc_ring.h"
#include <stdlib.h>

static inline size_t next_index(struct cc_ring *self, size_t index)
{
	size_t next_index = index + 1;
	if (cc_array_is_valid_index(self->data, next_index))
		return next_index;
	else
		return 0;
}

size_t cc_ring_elem_nums(struct cc_ring *self)
{
	if (self->write_index >= self->read_index)
		return self->write_index - self->read_index;
	else
		return self->data->elem_nums + self->write_index - self->read_index;
}

size_t cc_ring_space(struct cc_ring *self)
{
	return self->data->elem_nums - cc_ring_elem_nums(self) - 1;
}

int cc_ring_append(struct cc_ring *self, void *item)
{
	size_t write_index_next;

	write_index_next = next_index(self, self->write_index);
	if (write_index_next == self->read_index)
		return CC_RING_FULL;

	cc_array_set_unsafe(self->data, self->write_index, item);
	self->write_index = write_index_next;
	return 0;
}

int cc_ring_peek(struct cc_ring *self, void *item)
{
	if (self->read_index == self->write_index)
		return CC_RING_EMPTY;

	cc_array_get_unsafe(self->data, self->read_index, item);
	return 0;
}

int cc_ring_shift(struct cc_ring *self, void *item)
{
	if (cc_ring_peek(self, item) == CC_RING_EMPTY)
		return CC_RING_EMPTY;

	self->read_index = next_index(self, self->read_index);
	return 0;
}

int cc_ring_init(struct cc_ring *self, struct cc_array *data)
{
	self->data = data;
	self->read_index = 0;
	self->write_index = 0;
	return 0;
}

#ifndef NO_MALLOC

int cc_ring_new(struct cc_ring **self, size_t elem_nums, size_t elem_size)
{
	struct cc_ring *tmp;
	struct cc_array *data;

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;

	/// 1 element will be wasted, so pass `elem_nums + 1` to `cc_array_new`.
	if (cc_array_new(&data, elem_nums + 1, elem_size))
		goto fail2;
	if (cc_ring_init(tmp, data))
		goto fail3;

	*self = tmp;
	return 0;

fail3:
	cc_array_delete(data);
fail2:
	free(tmp);
fail1:
	return 1;
}

int cc_ring_delete(struct cc_ring *self)
{
	if (cc_array_delete(self->data))
		return 1;

	free(self);
	return 0;
}

#endif
