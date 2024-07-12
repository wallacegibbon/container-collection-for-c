#ifndef __CC_STACK_H
#define __CC_STACK_H

#include "cc_array.h"
#include "cc_common.h"
#include <stddef.h>

enum {
	CC_STACK_EMPTY = 0xFFE0,
	CC_STACK_FULL = 0xFFE1,
};

struct cc_stack {
	struct cc_array *data;
	size_t top;
};

#ifndef NO_MALLOC

int cc_stack_new(struct cc_stack **self, size_t elem_nums, size_t elem_size);
int cc_stack_delete(struct cc_stack *self);

#endif

int cc_stack_init(struct cc_stack *self, struct cc_array *data);

int cc_stack_push(struct cc_stack *self, void *item);
int cc_stack_pop(struct cc_stack *self, void *item);
int cc_stack_peek(struct cc_stack *self, void *item);

size_t cc_stack_elem_nums(struct cc_stack *self);
size_t cc_stack_space(struct cc_stack *self);

#endif
