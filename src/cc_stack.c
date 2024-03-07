#include "cc_stack.h"
#include "cc_array.h"
#include <stdlib.h>

int cc_stack_push(struct cc_stack *self, void *item) {
	if (!cc_array_set(self->data, self->top, item))
		return 0;

	self->top++;
	return 1;
}

int cc_stack_pop(struct cc_stack *self, void *item) {
	if (self->top == 0)
		return 0;

	self->top--;
	return cc_array_get(self->data, self->top, item);
}

void cc_stack_init(struct cc_stack *self, struct cc_array *data) {
	self->data = data;
	self->top = 0;
}

#ifndef NO_MALLOC

struct cc_stack *cc_stack_new(size_t elem_nums, size_t elem_size) {
	struct cc_stack *self;
	struct cc_array *data;

	self = malloc(sizeof(*self));
	if (self == NULL)
		return NULL;

	data = cc_array_new(elem_nums, elem_size);
	if (data == NULL)
		return NULL;

	cc_stack_init(self, data);

	return self;
}

void cc_stack_delete(struct cc_stack *self) {
	cc_array_delete(self->data);
	free(self);
}

#endif
