#include "cc_list_queue.h"
#include <stdlib.h>

struct cc_queue_i cc_list_queue_interface = {
	.enqueue = (cc_queue_enqueue_fn_t)cc_list_queue_enqueue,
	.dequeue = (cc_queue_dequeue_fn_t)cc_list_queue_dequeue,
	.peek = (cc_queue_peek_fn_t)cc_list_queue_peek,
};

static inline int translate_code(int code)
{
	return (code == CC_LIST_EMPTY) ? CC_QUEUE_EMPTY : code;
}

int cc_list_queue_enqueue(struct cc_list_queue *self, void *data)
{
	return cc_list_insert_tail(self->list, data);
}

int cc_list_queue_dequeue(struct cc_list_queue *self, void **result)
{
	return translate_code(cc_list_remove_head(self->list, result));
}

int cc_list_queue_peek(struct cc_list_queue *self, void **result)
{
	return translate_code(cc_list_get_head(self->list, result));
}

int cc_list_queue_new(struct cc_list_queue **self)
{
	struct cc_list_queue *tmp;
	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;

	tmp->interface = &cc_list_queue_interface;
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

int cc_list_queue_delete(struct cc_list_queue *self)
{
	if (cc_list_delete(self->list))
		return 1;

	free(self);
	return 0;
}
