#include "cc_list_stack.h"
#include <stdint.h>
#include <stdlib.h>

struct cc_stack_i cc_list_stack_interface = {
	.push = (cc_stack_push_fn_t)cc_list_stack_push,
	.pop = (cc_stack_pop_fn_t)cc_list_stack_pop,
	.peek = (cc_stack_peek_fn_t)cc_list_stack_peek,
};

static inline int translate_code(int code) {
	return (code == CC_LIST_EMPTY) ? CC_STACK_EMPTY : code;
}

int cc_list_stack_push(struct cc_list_stack *self, void *data) {
	return cc_list_insert_head(self->list, data);
}

int cc_list_stack_pop(struct cc_list_stack *self, void **result) {
	return translate_code(cc_list_remove_head(self->list, result));
}

int cc_list_stack_peek(struct cc_list_stack *self, void **result) {
	return translate_code(cc_list_get_head(self->list, result));
}

int cc_list_stack_new(struct cc_list_stack **self) {
	struct cc_list_stack *tmp;
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

int cc_list_stack_delete(struct cc_list_stack *self) {
	if (cc_list_delete(self->list))
		return 1;

	free(self);
	return 0;
}
