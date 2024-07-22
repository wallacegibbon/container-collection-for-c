#ifndef __CC_LIST_QUEUE_H
#define __CC_LIST_QUEUE_H

#include "cc_list.h"
#include "cc_queue.h"

struct cc_list_queue {
	struct cc_queue_i *interface;
	struct cc_list *data;
	struct cc_list_cursor cursor;
};

int cc_list_queue_new(struct cc_list_queue **self, cc_delete_fn_t remove);
int cc_list_queue_delete(struct cc_list_queue *self);

int cc_list_queue_init(struct cc_list_queue *self, struct cc_list *data, cc_delete_fn_t remove);

int cc_list_queue_enqueue(struct cc_list_queue *self, void *data);
int cc_list_queue_dequeue(struct cc_list_queue *self, void **result);
int cc_list_queue_peek(struct cc_list_queue *self, void **result);

#endif
