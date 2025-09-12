#ifndef __CC_LIST_QUEUE_H
#define __CC_LIST_QUEUE_H

#include "cc_list.h"
#include "cc_queue.h"

typedef struct cc_ListQueue cc_ListQueue;

struct cc_ListQueue {
	cc_QueueI *interface;
	cc_List *list;
};

int	cc_list_queue_new(cc_ListQueue **self);
int	cc_list_queue_delete(cc_ListQueue *self);
int	cc_list_queue_enqueue(cc_ListQueue *self, void *data);
int	cc_list_queue_dequeue(cc_ListQueue *self, void **result);
int	cc_list_queue_peek(cc_ListQueue *self, void **result);

#endif
