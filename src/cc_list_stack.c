#include "cc_list_stack.h"
#include <stdint.h>
#include <stdlib.h>

struct cc_StackI cc_list_stack_interface = {
	.push = (cc_StackPushFn)cc_list_stack_push,
	.pop = (cc_StackPopFn)cc_list_stack_pop,
	.peek = (cc_StackPeekFn)cc_list_stack_peek,
};

static inline int translate_code(int code)
{
	return (code == CC_LIST_EMPTY) ? CC_STACK_EMPTY : code;
}

int cc_list_stack_push(cc_ListStack *self, void *data)
{
	return cc_list_insert_head(self->list, data);
}

int cc_list_stack_pop(cc_ListStack *self, void **result)
{
	return translate_code(cc_list_remove_head(self->list, result));
}

int cc_list_stack_peek(cc_ListStack *self, void **result)
{
	return translate_code(cc_list_get_head(self->list, result));
}

int cc_list_stack_new(cc_ListStack **self)
{
	cc_ListStack *tmp;
	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;

	tmp->interface = &cc_list_stack_interface;
	if (cc_list_new(&tmp->list))
		goto fail2;

	*self = tmp;
	return 0;
fail3:
	cc_list_delete(tmp->list);
fail2:
	free(tmp);
fail1:
	return 1;
}

int cc_list_stack_delete(cc_ListStack *self)
{
	if (cc_list_delete(self->list))
		return 1;

	free(self);
	return 0;
}
