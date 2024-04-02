#include "cc_array.h"
#include <stdlib.h>
#include <string.h>

static inline void _cc_array_get(struct cc_array *self, size_t index, void *result) {
	memcpy(result, self->buffer + index * self->elem_size, self->elem_size);
}

static inline void _cc_array_get_ref(struct cc_array *self, size_t index, void **ref) {
	*ref = self->buffer + index * self->elem_size;
}

static inline void _cc_array_set(struct cc_array *self, size_t index, void *value) {
	memcpy(self->buffer + index * self->elem_size, value, self->elem_size);
}

int cc_array_swap(struct cc_array *self, size_t i, size_t j) {
	uint8_t tmp[self->elem_size];
	_cc_array_get(self, i, tmp);
	memcpy(self->buffer + i * self->elem_size, self->buffer + j * self->elem_size, self->elem_size);
	_cc_array_set(self, j, tmp);
	return 0;
}

int cc_array_cmp(struct cc_array *self, cc_cmp_fn cmp, size_t i, size_t j) {
	return cmp(self->buffer + i * self->elem_size, self->buffer + j * self->elem_size);
}

int cc_array_is_valid_index(struct cc_array *self, size_t index) {
	return index < self->elem_nums;
}

int cc_array_get_unsafe(struct cc_array *self, size_t index, void *result) {
	_cc_array_get(self, index, result);
	return 0;
}

int cc_array_get_ref_unsafe(struct cc_array *self, size_t index, void **ref) {
	_cc_array_get_ref(self, index, ref);
	return 0;
}

int cc_array_get(struct cc_array *self, size_t index, void *result) {
	if (result == NULL)
		return 1;
	if (index >= self->elem_nums)
		return 2;

	_cc_array_get(self, index, result);
	return 0;
}

int cc_array_get_ref(struct cc_array *self, size_t index, void **ref) {
	if (try_reset_double_p(ref))
		return 1;
	if (index >= self->elem_nums)
		return 2;

	_cc_array_get_ref(self, index, ref);
	return 0;
}

int cc_array_set_unsafe(struct cc_array *self, size_t index, void *value) {
	_cc_array_set(self, index, value);
	return 0;
}

int cc_array_set(struct cc_array *self, size_t index, void *value) {
	if (index >= self->elem_nums)
		return 1;

	_cc_array_set(self, index, value);
	return 0;
}

int cc_array_reverse(struct cc_array *self, size_t start, size_t end) {
	size_t middle, i;

	if (start == end)
		return 1;
	if (start >= self->elem_nums)
		return 2;
	if (end > self->elem_nums)
		end = self->elem_nums;

	middle = start + (end - start) / 2;
	for (i = start; i < middle; i++)
		cc_array_swap(self, i, end - i - 1 + start);

	return 0;
}

int cc_array_init(struct cc_array *self, uint8_t *buffer, size_t elem_nums, size_t elem_size) {
	self->elem_nums = elem_nums;
	self->elem_size = elem_size;
	self->buffer = buffer;
	return 0;
}

#ifndef NO_MALLOC

struct cc_array *cc_array_new(size_t elem_nums, size_t elem_size) {
	struct cc_array *self;
	uint8_t *buffer;

	self = malloc(sizeof(*self));
	if (self == NULL)
		goto fail1;

	buffer = malloc(elem_nums * elem_size);
	if (buffer == NULL)
		goto fail2;

	if (cc_array_init(self, buffer, elem_nums, elem_size))
		goto fail3;

	return self;

fail3:
	free(buffer);
fail2:
	free(self);
fail1:
	return NULL;
}

int cc_array_delete(struct cc_array *self) {
	free(self->buffer);
	free(self);
	return 0;
}

#endif

static const struct cc_iter_i iterator_interface = {
	.next = (cc_iter_next_fn)cc_array_iter_next,
};

int cc_array_iter_init(struct cc_array_iter *self, struct cc_array *data) {
	if (data == NULL)
		return 1;

	self->iterator = (struct cc_iter_i **)&iterator_interface;
	self->data = data;
	self->cursor = 0;
	return 0;
}

int cc_array_iter_next(struct cc_array_iter *self, void **item, size_t *index) {
	if (try_reset_double_p(item))
		return 1;
	if (cc_array_get_ref(self->data, self->cursor, item))
		return 2;

	if (index != NULL)
		*index = self->cursor;

	self->cursor++;
	return 0;
}
