#ifndef __CC_RING_H
#define __CC_RING_H

#include "cc_array.h"
#include "cc_common.h"
#include "cc_queue.h"
#include <stddef.h>

struct cc_ring {
	struct cc_queue_i *interface;
	struct cc_array *data;
	size_t read_index;
	size_t write_index;
};

int cc_ring_new(struct cc_ring **self, size_t elem_nums, size_t elem_size);
int cc_ring_delete(struct cc_ring *self);

int cc_ring_init(struct cc_ring *self, struct cc_array *data);

int cc_ring_enqueue(struct cc_ring *self, void *item);
int cc_ring_dequeue(struct cc_ring *self, void *item);
int cc_ring_peek(struct cc_ring *self, void *item);

size_t cc_ring_elem_nums(struct cc_ring *self);
size_t cc_ring_space(struct cc_ring *self);

#endif
