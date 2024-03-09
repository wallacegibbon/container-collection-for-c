#include "cc_array.h"
#include "cc_common.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static inline int _cc_array_get(struct cc_array *self, size_t index, void *result) {
	memcpy(result, self->buffer + index * self->elem_size, self->elem_size);
	return 1;
}

static inline int _cc_array_set(struct cc_array *self, size_t index, void *value) {
	memcpy(self->buffer + index * self->elem_size, value, self->elem_size);
	return 1;
}

/// Please ensure this function is not inlined since it uses VLA.
/// Or memory consumption could be huge when you call this function in a long loop.
void cc_array_swap(struct cc_array *self, size_t i, size_t j) {
	uint8_t tmp[self->elem_size];
	_cc_array_get(self, i, tmp);
	memcpy(self->buffer + i * self->elem_size, self->buffer + j * self->elem_size, self->elem_size);
	_cc_array_set(self, j, tmp);
}

int cc_array_cmp(struct cc_array *self, cc_cmp_fn cmp, size_t i, size_t j) {
	return cmp(self->buffer + i * self->elem_size, self->buffer + j * self->elem_size);
}

int cc_array_check_index(struct cc_array *self, size_t index) {
	return index < self->elem_nums;
}

void cc_array_get_unsafe(struct cc_array *self, size_t index, void *result) {
	_cc_array_get(self, index, result);
}

int cc_array_get(struct cc_array *self, size_t index, void *result) {
	if (index >= self->elem_nums)
		return 0;
	else
		return _cc_array_get(self, index, result);
}

void cc_array_set_unsafe(struct cc_array *self, size_t index, void *value) {
	_cc_array_set(self, index, value);
}

int cc_array_set(struct cc_array *self, size_t index, void *value) {
	if (index >= self->elem_nums)
		return 0;
	else
		return _cc_array_set(self, index, value);
}

void cc_array_init(struct cc_array *self, uint8_t *buffer, size_t elem_nums, size_t elem_size) {
	self->elem_nums = elem_nums;
	self->elem_size = elem_size;
	self->buffer = buffer;
}

#ifndef NO_MALLOC

struct cc_array *cc_array_new(size_t elem_nums, size_t elem_size) {
	struct cc_array *self;
	uint8_t *buffer;

	self = malloc(sizeof(*self));
	if (self == NULL)
		goto fail1;

	buffer = malloc(sizeof(elem_nums * elem_size));
	if (buffer == NULL)
		goto fail2;

	cc_array_init(self, buffer, elem_nums, elem_size);
	return self;

fail2:
	free(self);
fail1:
	return NULL;
}

void cc_array_delete(struct cc_array *self) {
	free(self->buffer);
	free(self);
}

#endif

void cc_array_iter_init(struct cc_array_iter *self, struct cc_array *data) {
	self->data = data;
	self->cursor = 0;
}

int cc_array_iter_next(struct cc_array_iter *self, void *item) {
	if (!cc_array_get(self->data, self->cursor, item))
		return 0;

	self->cursor++;
	return 1;
}
