#include "cc_stack.h"
#include <stdlib.h>

int cc_stack_push(struct cc_stack *self, void *item)
{
	if (cc_array_set(self->data, self->top, item) == CC_ARRAY_OUT_OF_RANGE)
		return CC_STACK_FULL;

	self->top++;
	return 0;
}

int cc_stack_pop(struct cc_stack *self, void *item)
{
	if (self->top == 0)
		return CC_STACK_EMPTY;

	self->top--;
	cc_array_get_unsafe(self->data, self->top, item);
	return 0;
}

int cc_stack_peek(struct cc_stack *self, void *item)
{
	if (self->top == 0)
		return CC_STACK_EMPTY;

	cc_array_get_unsafe(self->data, self->top - 1, item);
	return 0;
}

size_t cc_stack_elem_nums(struct cc_stack *self)
{
	return self->top;
}

size_t cc_stack_space(struct cc_stack *self)
{
	return self->data->elem_nums - self->top;
}

int cc_stack_init(struct cc_stack *self, struct cc_array *data)
{
	self->data = data;
	self->top = 0;
	return 0;
}

#ifndef NO_MALLOC

int cc_stack_new(struct cc_stack **self, size_t elem_nums, size_t elem_size)
{
	struct cc_stack *tmp;
	struct cc_array *data;

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;
	if (cc_array_new(&data, elem_nums, elem_size))
		goto fail2;
	if (cc_stack_init(tmp, data))
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

int cc_stack_delete(struct cc_stack *self)
{
	if (cc_array_delete(self->data))
		return 1;

	free(self);
	return 0;
}

#endif
