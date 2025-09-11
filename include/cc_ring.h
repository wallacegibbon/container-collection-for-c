#ifndef __CC_RING_H
#define __CC_RING_H

#include "cc_array.h"
#include "cc_common.h"
#include "cc_queue.h"
#include <stddef.h>

typedef struct cc_Ring cc_Ring;

struct cc_Ring {
	cc_QueueI *interface;
	cc_Array *data;
	size_t read_index;
	size_t write_index;
};

int cc_ring_new(cc_Ring **self, size_t elem_nums, size_t elem_size);
int cc_ring_delete(cc_Ring *self);
int cc_ring_init(cc_Ring *self, cc_Array *data);
int cc_ring_enqueue(cc_Ring *self, void *item);
int cc_ring_dequeue(cc_Ring *self, void *item);
int cc_ring_peek(cc_Ring *self, void *item);
size_t cc_ring_elem_nums(cc_Ring *self);
size_t cc_ring_space(cc_Ring *self);

#endif
