#ifndef __CC_LIST_QUEUE_H
#define __CC_LIST_QUEUE_H

#include "cc_list.h"
#include "cc_queue.h"

struct cc_list_queue {
	struct cc_queue_i *interface;
	struct cc_list *list;
};

int cc_list_queue_new(struct cc_list_queue **self);
int cc_list_queue_delete(struct cc_list_queue *self);

int cc_list_queue_enqueue(struct cc_list_queue *self, void *data);
int cc_list_queue_dequeue(struct cc_list_queue *self, void **result);
int cc_list_queue_peek(struct cc_list_queue *self, void **result);

#endif
