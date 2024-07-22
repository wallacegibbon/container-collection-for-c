#include "cc_list_stack.h"
#include <stdlib.h>

struct cc_stack_i cc_list_stack_interface = {
	.push = (cc_stack_push_fn_t)cc_list_stack_push,
	.pop = (cc_stack_pop_fn_t)cc_list_stack_pop,
	.peek = (cc_stack_peek_fn_t)cc_list_stack_peek,
};

int cc_list_stack_init(struct cc_list_stack *self, struct cc_list *data, cc_delete_fn_t remove)
{
	self->interface = &cc_list_stack_interface;
	self->data = data;
	if (cc_list_cursor_init(&self->cursor, data, remove))
		return 1;

	return 0;
}

int cc_list_stack_new(struct cc_list_stack **self, cc_delete_fn_t remove)
{
	struct cc_list_stack *tmp;
	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;
	if (cc_list_new(&tmp->data))
		goto fail2;
	if (cc_list_stack_init(tmp, tmp->data, remove))
		goto fail3;

	*self = tmp;
	return 0;
fail3:
	cc_list_delete(tmp->data);
fail2:
	free(tmp);
fail1:
	return 1;
}

int cc_list_stack_delete(struct cc_list_stack *self)
{
	return 0;
}

int cc_list_stack_push(struct cc_list_stack *self, void *data)
{
	return 0;
}

int cc_list_stack_pop(struct cc_list_stack *self, void **result)
{
	return 0;
}

int cc_list_stack_peek(struct cc_list_stack *self, void **result)
{
	return 0;
}
