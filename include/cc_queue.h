#ifndef __CC_QUEUE_H
#define __CC_QUEUE_H

typedef int (*cc_queue_enqueue_fn_t)(void *self, void *data);
typedef int (*cc_queue_dequeue_fn_t)(void *self, void **result);
typedef int (*cc_queue_peek_fn_t)(void *self, void **result);

struct cc_queue_i {
	cc_queue_enqueue_fn_t enqueue;
	cc_queue_dequeue_fn_t dequeue;
	cc_queue_peek_fn_t peek;
};

static inline int cc_queue_enqueue(void *self, void *data)
{
	return (*(struct cc_queue_i **)self)->enqueue(self, data);
}

static inline int cc_queue_dequeue(void *self, void **result)
{
	return (*(struct cc_queue_i **)self)->dequeue(self, result);
}

static inline int cc_queue_peek(void *self, void **result)
{
	return (*(struct cc_queue_i **)self)->peek(self, result);
}

#endif
