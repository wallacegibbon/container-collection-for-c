#ifndef __CC_ARRAY_STACK_H
#define __CC_ARRAY_STACK_H

#include "cc_array.h"
#include "cc_common.h"
#include "cc_stack.h"
#include <stddef.h>

typedef struct cc_ArrayStack cc_ArrayStack;

struct cc_ArrayStack {
	cc_StackI *interface;
	cc_Array *data;
	size_t top;
};

int cc_array_stack_new(cc_ArrayStack **self, size_t elem_nums, size_t elem_size);
int cc_array_stack_delete(cc_ArrayStack *self);
int cc_array_stack_init(cc_ArrayStack *self, cc_Array *data);
int cc_array_stack_push(cc_ArrayStack *self, void *item);
int cc_array_stack_pop(cc_ArrayStack *self, void *item);
int cc_array_stack_peek(cc_ArrayStack *self, void *item);
size_t cc_array_stack_elem_nums(cc_ArrayStack *self);
size_t cc_array_stack_space(cc_ArrayStack *self);

#endif
