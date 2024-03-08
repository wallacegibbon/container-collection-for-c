#ifndef __CC_ARRAY_H
#define __CC_ARRAY_H

#include "cc_common.h"
#include <stddef.h>
#include <stdint.h>

struct cc_array {
	uint8_t *buffer;
	size_t elem_nums;
	size_t elem_size;
};

void cc_array_init(struct cc_array *self, uint8_t *buffer, size_t elem_nums, size_t elem_size);

#ifndef NO_MALLOC

struct cc_array *cc_array_new(size_t elem_nums, size_t elem_size);
void cc_array_delete(struct cc_array *self);

#endif

int cc_array_check(struct cc_array *self, size_t index);

void cc_array_get_unsafe(struct cc_array *self, size_t index, void *result);
int cc_array_get(struct cc_array *self, size_t index, void *result);

void cc_array_set_unsafe(struct cc_array *self, size_t index, void *value);
int cc_array_set(struct cc_array *self, size_t index, void *value);

int cc_array_sort(struct cc_array *self, cc_cmp_fn);

struct cc_array_iter {
	struct cc_array *data;
	size_t cursor;
};

void cc_array_iter_init(struct cc_array_iter *self, struct cc_array *data);
int cc_array_iter_next(struct cc_array_iter *self, void *item);

#endif
