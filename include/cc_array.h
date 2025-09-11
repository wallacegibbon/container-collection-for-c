#ifndef __CC_ARRAY_H
#define __CC_ARRAY_H

#include "cc_common.h"
#include "cc_iter.h"
#include <stddef.h>

enum {
	CC_ARRAY_OUT_OF_RANGE = 0xFFD0,
};

typedef struct cc_Array cc_Array;

struct cc_Array {
	unsigned char *data;
	size_t elem_nums;
	size_t elem_size;
};

int cc_array_new(cc_Array **self, size_t elem_nums, size_t elem_size);
int cc_array_delete(cc_Array *self);
int cc_array_delete_keep_data(cc_Array *self);

int cc_array_init(cc_Array *self, unsigned char *data, size_t elem_nums,
		size_t elem_size);

/* These `_unsafe` functions do not check the boundaries of arrays. */

void cc_array_get_unsafe(cc_Array *self, size_t index, void *result);
void cc_array_get_ref_unsafe(cc_Array *self, size_t index, void **ref);
void cc_array_set_unsafe(cc_Array *self, size_t index, void *value);

int cc_array_get(cc_Array *self, size_t index, void *result);
int cc_array_get_ref(cc_Array *self, size_t index, void **ref);
int cc_array_set(cc_Array *self, size_t index, void *value);

/* Check whether the index has exceeded the boundary of the array. */
int cc_array_is_valid_index(cc_Array *self, size_t index);
int cc_array_cmp(cc_Array *self, cc_CmpFn cmp, size_t i, size_t j);
int cc_array_reverse(cc_Array *self, size_t start, size_t end);
void cc_array_swap(cc_Array *self, size_t i, size_t j);

typedef struct cc_ArrayIter cc_ArrayIter;

struct cc_ArrayIter {
	cc_IterI *iterator;
	cc_Array *data;
	size_t cursor;
};

int cc_array_iter_init(cc_ArrayIter *self, cc_Array *data);
int cc_array_iter_next(cc_ArrayIter *self, void **item, size_t *index);

#endif
