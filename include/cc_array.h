#ifndef __CC_ARRAY_H
#define __CC_ARRAY_H

#include "cc_common.h"
#include "cc_iter.h"
#include <stddef.h>

enum {
    CC_ARRAY_OUT_OF_RANGE = 0xFFD0,
};

///-----------------------------------------------------------------------------
/// Generic array for data of different length
///-----------------------------------------------------------------------------
struct cc_array {
    unsigned char *data;
    size_t elem_nums;
    size_t elem_size;
};

int cc_array_new(struct cc_array **self, size_t elem_nums, size_t elem_size);
int cc_array_delete(struct cc_array *self);
int cc_array_delete_keep_data(struct cc_array *self);

int cc_array_init(struct cc_array *self, unsigned char *data, size_t elem_nums, size_t elem_size);

/// These `_unsafe` functions do not check the boundaries of arrays.

void cc_array_get_unsafe(struct cc_array *self, size_t index, void *result);
void cc_array_get_ref_unsafe(struct cc_array *self, size_t index, void **ref);
void cc_array_set_unsafe(struct cc_array *self, size_t index, void *value);

int cc_array_get(struct cc_array *self, size_t index, void *result);
int cc_array_get_ref(struct cc_array *self, size_t index, void **ref);
int cc_array_set(struct cc_array *self, size_t index, void *value);

/// Check whether the index has exceeded the boundary of the array.
int cc_array_is_valid_index(struct cc_array *self, size_t index);
int cc_array_cmp(struct cc_array *self, cc_cmp_fn_t cmp, size_t i, size_t j);
int cc_array_reverse(struct cc_array *self, size_t start, size_t end);
void cc_array_swap(struct cc_array *self, size_t i, size_t j);

///-----------------------------------------------------------------------------
/// The iterator for the generic array
///-----------------------------------------------------------------------------
struct cc_array_iter {
    struct cc_iter_i *iterator;
    struct cc_array *data;
    size_t cursor;
};

int cc_array_iter_init(struct cc_array_iter *self, struct cc_array *data);
int cc_array_iter_next(struct cc_array_iter *self, void **item, size_t *index);

#endif
