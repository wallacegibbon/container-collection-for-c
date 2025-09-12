#include "cc_array_stack.h"
#include <stdlib.h>

struct cc_StackI cc_array_stack_interface = {
	.push = (cc_StackPushFn)cc_array_stack_push,
	.pop = (cc_StackPopFn)cc_array_stack_pop,
	.peek = (cc_StackPeekFn)cc_array_stack_peek,
};

int
cc_array_stack_push(cc_ArrayStack *self, void *item)
{
	if (cc_array_set(self->data, self->top, item) == CC_ARRAY_OUT_OF_RANGE)
		return CC_STACK_FULL;

	self->top++;
	return 0;
}

int
cc_array_stack_pop(cc_ArrayStack *self, void *item)
{
	if (self->top == 0)
		return CC_STACK_EMPTY;

	self->top--;
	cc_array_get_unsafe(self->data, self->top, item);
	return 0;
}

int
cc_array_stack_peek(cc_ArrayStack *self, void *item)
{
	if (self->top == 0)
		return CC_STACK_EMPTY;

	cc_array_get_unsafe(self->data, self->top - 1, item);
	return 0;
}

size_t
cc_array_stack_elem_nums(cc_ArrayStack *self)
{
	return self->top;
}

size_t
cc_array_stack_space(cc_ArrayStack *self)
{
	return self->data->elem_nums - self->top;
}

int
cc_array_stack_init(cc_ArrayStack *self, cc_Array *data)
{
	self->interface = &cc_array_stack_interface;
	self->data = data;
	self->top = 0;
	return 0;
}

int
cc_array_stack_new(cc_ArrayStack **self, size_t elem_nums, size_t elem_size)
{
	cc_ArrayStack *tmp;
	cc_Array *data;

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;
	if (cc_array_new(&data, elem_nums, elem_size))
		goto fail2;
	if (cc_array_stack_init(tmp, data))
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

int
cc_array_stack_delete(cc_ArrayStack *self)
{
	if (cc_array_delete(self->data))
		return 1;

	free(self);
	return 0;
}
