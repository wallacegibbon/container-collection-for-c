#ifndef __CC_QUEUE_H
#define __CC_QUEUE_H

typedef int (*cc_EnqueueFn)(void *self, void *data);
typedef int (*cc_DequeueFn)(void *self, void **result);
typedef int (*cc_QueuePeekFn)(void *self, void **result);

typedef struct cc_QueueI cc_QueueI;

struct cc_QueueI {
	cc_EnqueueFn enqueue;
	cc_DequeueFn dequeue;
	cc_QueuePeekFn peek;
};

enum {
	CC_QUEUE_EMPTY = 0xFD10,
	CC_QUEUE_FULL = 0xFD20,
};

static inline int
cc_queue_enqueue(void *self, void *data)
{
	return (*(cc_QueueI **)self)->enqueue(self, data);
}

static inline int
cc_queue_dequeue(void *self, void **result)
{
	return (*(cc_QueueI **)self)->dequeue(self, result);
}

static inline int
cc_queue_peek(void *self, void **result)
{
	return (*(cc_QueueI **)self)->peek(self, result);
}

#endif
