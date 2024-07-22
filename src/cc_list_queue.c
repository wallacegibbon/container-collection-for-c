#include "cc_list_queue.h"
#include <stdlib.h>

struct cc_queue_i cc_list_queue_interface = {
	.enqueue = (cc_queue_enqueue_fn_t)cc_list_queue_enqueue,
	.dequeue = (cc_queue_dequeue_fn_t)cc_list_queue_dequeue,
	.peek = (cc_queue_peek_fn_t)cc_list_queue_peek,
};

int cc_list_queue_init(struct cc_list_queue *self, struct cc_list *data, cc_delete_fn_t remove)
{
	self->interface = &cc_list_queue_interface;
	self->data = data;
	if (cc_list_cursor_init(&self->cursor, data, remove))
		return 1;

	return 0;
}

int cc_list_queue_new(struct cc_list_queue **self, cc_delete_fn_t remove)
{
	struct cc_list_queue *tmp;
	tmp = malloc(sizeof(*tmp));
	if (tmp == NULL)
		goto fail1;
	if (cc_list_new(&tmp->data))
		goto fail2;
	if (cc_list_queue_init(tmp, tmp->data, remove))
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

int cc_list_queue_delete(struct cc_list_queue *self)
{
	return 0;
}

int cc_list_queue_enqueue(struct cc_list_queue *self, void *data)
{
	return 0;
}

int cc_list_queue_dequeue(struct cc_list_queue *self, void **result)
{
	return 0;
}

int cc_list_queue_peek(struct cc_list_queue *self, void **result)
{
	return 0;
}
