#ifndef __CC_LIST_STACK_H
#define __CC_LIST_STACK_H

#include "cc_list.h"
#include "cc_stack.h"

struct cc_list_stack {
	struct cc_stack_i *interface;
	struct cc_list *data;
	struct cc_list_cursor cursor;
};

int cc_list_stack_new(struct cc_list_stack **self, cc_delete_fn_t remove);
int cc_list_stack_delete(struct cc_list_stack *self);

int cc_list_stack_init(struct cc_list_stack *self, struct cc_list *data, cc_delete_fn_t remove);

int cc_list_stack_push(struct cc_list_stack *self, void *data);
int cc_list_stack_pop(struct cc_list_stack *self, void **result);
int cc_list_stack_peek(struct cc_list_stack *self, void **result);

#endif
