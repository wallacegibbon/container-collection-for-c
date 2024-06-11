#include "cc_stack.h"
#include "cc_array.h"
#include <stdlib.h>

int cc_stack_push(struct cc_stack *self, void *item)
{
	if (cc_array_set(self->data, self->top, item))
		return 1;

	self->top++;
	return 0;
}

int cc_stack_pop(struct cc_stack *self, void *item)
{
	if (self->top == 0)
		return 1;

	self->top--;
	return cc_array_get(self->data, self->top, item);
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
	int code = 0;

	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL) {
		code = 1;
		goto fail1;
	}

	code = cc_array_new(&data, elem_nums, elem_size);
	if (code)
		goto fail2;

	code = cc_stack_init(tmp, data);
	if (code)
		goto fail3;

	*self = tmp;
	return 0;

fail3:
	code = cc_array_delete(data);
fail2:
	free(tmp);
fail1:
	return code;
}

int cc_stack_delete(struct cc_stack *self)
{
	if (cc_array_delete(self->data))
		return 1;

	free(self);
	return 0;
}

#endif
